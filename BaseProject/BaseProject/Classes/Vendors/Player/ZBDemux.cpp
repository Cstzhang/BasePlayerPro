//
//  ZBDemux.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "ZBDemux.hpp"
#include "ZBLog.hpp"

extern "C"{
#include <libavformat/avformat.h>
}

//Fractional to floating point
static  double  r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0.:(double)r.num/(double)r.den;
}


void ZBDemux::Close()
{
    mux.lock();
    if(ic)
    {
        avformat_close_input(&ic);
    }
    mux.unlock();
}

//seek pos = 0.0 ~ 1.0
bool ZBDemux::Seek(double pos)
{
    if (pos <0 || pos > 1)
    {
        ZLOGE("Seek vale must 0.0~1.0");
        return false;
        
    }
    bool re = false;
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return  false;
    }
    //Clear the read buffer
    avformat_flush(ic);
    
    long long seekPts = 0;
    seekPts = ic->streams[videoStream]->duration*pos;
    //Jump back to the keyframe
    re = av_seek_frame(ic,videoStream,seekPts,AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);
    
    mux.unlock();
    return  re;
    
    
}

//Open files or stream media rtmp http rtsp
bool ZBDemux::Open(const char *url)
{
    ZLOGI("Open file %s begin \n",url);
    Close();
    mux.lock();
    // 1.open fiel or stream  ic = 0
    int re = avformat_open_input(&ic,url,0,0);
    if(re != 0 )
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        ZLOGE("FFDemux open %s failed! \n",url);
        return false;
    }
    ZLOGI("FFDemux open %s success! \n",url);
    
    //2 Read file information
    re = avformat_find_stream_info(ic,0);
    if(re != 0 )
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf));
        ZLOGE("avformat_find_stream_info %s failed! \n",url);
        return false;
    }
    
    this->totalMs = ic->duration/(AV_TIME_BASE/1000);
    
    mux.unlock();
    ZLOGI("total ms = %lld !",totalMs);
    
    GetVPara();
    GetAPara();
    return true;
}

//Read the video parameter
ZParameter ZBDemux::GetVPara()
{
    mux.lock();
    if (!ic) {
        mux.unlock();
        ZLOGE("GetVPara failed! ic is NULL！\n");
        return ZParameter();
    }
    //Gets the video stream index
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        ZLOGE("av_find_best_stream failed!\n");
        return ZParameter();
    }
    videoStream = re;
    ZParameter para;
    para.para = ic->streams[re]->codecpar;
    mux.unlock();
    return para;
}

ZParameter ZBDemux::GetAPara(){
    mux.lock();
    if (!ic) {
        mux.unlock();
        ZLOGE("GetVPara failed! ic is NULL！\n");
        return ZParameter();
    }
    //Gets the audio stream index
    int re = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0);
    if (re < 0) {
        mux.unlock();
        ZLOGE("av_find_best_stream failed!\n");
        return ZParameter();
    }
    audioStream = re;
    ZParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;
    mux.unlock();
    return para;
    
}



//Reads a frame of data, which is cleaned by the caller
ZBData ZBDemux::Read()
{
    mux.lock();
    if(!ic)
    {
        mux.unlock();
        return ZBData();
    }
    
    ZBData d;
    AVPacket *pkt = av_packet_alloc();
    int re = av_read_frame(ic,pkt);
    if(re != 0)
    {
        mux.unlock();
        av_packet_free(&pkt);
        return ZBData();
    }
    //XLOGI("pack size is %d ptss %lld",pkt->size,pkt->pts);
    d.data = (unsigned char*)pkt;
    d.size = pkt->size;
    if(pkt->stream_index == audioStream)
    {
        d.isAudio = true;
    }
    else if(pkt->stream_index == videoStream)
    {
        d.isAudio = false;
    }
    else
    {
        mux.unlock();
        //free
        av_packet_free(&pkt);
        return ZBData();
    }
    
    //Conversion pts
    pkt->pts = pkt->pts * (1000*r2d(ic->streams[pkt->stream_index]->time_base));
    pkt->dts = pkt->dts * (1000*r2d(ic->streams[pkt->stream_index]->time_base));
    d.pts = (int)pkt->pts;
    //XLOGE("demux pts %d",d.pts);
    mux.unlock();
    return d;
    
}
//Initialize
ZBDemux::ZBDemux()
{
    static bool isFirst = true;
    if(isFirst)
    {
        // 0. register ffmpeg
        isFirst = false;
        //  Initialize libavformat and register all the muxers, demuxers and protocols
//        av_register_all();
        
        //Register all the decoders
//        avcodec_register_all();
        
        //Initialization network
        avformat_network_init();
        ZLOGI("register ffmpeg! \n");
    }
    
}
