//
//  ZBDecode.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "ZBDecode.hpp"
#include "ZBLog.hpp"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavcodec/jni.h>
}


void ZBDecode::InitHard(void *vm)
{
    av_jni_set_java_vm(vm,0);
}


void  ZBDecode::Clear()
{
    IDecode::Clear();
    mux.lock();
    if(codec)
    {
        avcodec_flush_buffers(codec);
    }
    mux.unlock();
}

void ZBDecode::Close()
{
    IDecode::Clear();
    mux.lock();
    pts = 0;
    if(frame)
        av_frame_free(&frame);
    if(codec)
    {
        avcodec_close(codec);
        avcodec_free_context(&codec);
    }
    mux.unlock();
}

bool ZBDecode::Open(ZBParameters para , bool isHard)
{
    Close();
    if(!para.para) return false;
    AVCodecParameters *p = para.para;
    
    //1 Search decoder
    AVCodec *cd = avcodec_find_decoder(p->codec_id);
    if(isHard)
    {
        cd = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    
    if(!cd)
    {
        ZLOGE("avcodec_find_decoder %d failed!  %d /n",p->codec_id,isHard);
        return false;
    }
    ZLOGI("avcodec_find_decoder success %d! /n",isHard);
    
    mux.lock();
    //2 Create the decoding context and copy the parameters
    codec = avcodec_alloc_context3(cd);
    avcodec_parameters_to_context(codec,p);
    
    codec->thread_count = 8;
    //3 Turn on the decoder
    int re = avcodec_open2(codec,0,0);
    if(re != 0)
    {
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(re,buf,sizeof(buf)-1);
        ZLOGE("%s",buf);
        return false;
    }
    
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        this->isAudio = false;
    }
    else
    {
        this->isAudio = true;
    }
    mux.unlock();
    ZLOGI("avcodec_open2 success! /n");
    return true;
}


bool ZBDecode::SendPacket(ZBData pkt)
{
    if(pkt.size<=0 || !pkt.data)return false;
    mux.lock();
    if(!codec)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec,(AVPacket*)pkt.data);
    mux.unlock();
    if(re != 0)
    {
        return false;
    }
    return true;
}

//Gets the decoding results from the thread
ZBData ZBDecode::RecvFrame()
{
    mux.lock();
    if(!codec)
    {
        mux.unlock();
        return ZBData();
    }
    if(!frame)
    {
        frame = av_frame_alloc();
    }
    int re = avcodec_receive_frame(codec,frame);
    if(re != 0)
    {
        mux.unlock();
        return ZBData();
    }
    ZBData d;
    d.data = (unsigned char *)frame;
    if(codec->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        d.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2])*frame->height;
        d.width = frame->width;
        d.height = frame->height;
    }
    else
    {
        //Number of sample bytes * number of single channel samples * number of channels
        d.size = av_get_bytes_per_sample((AVSampleFormat)frame->format)*frame->nb_samples*2;
    }
    d.format = frame->format;
    //if(!isAudio)
    //    XLOGE("data format is %d",frame->format);
    memcpy(d.datas,frame->data,sizeof(d.datas));
    d.pts = frame->pts;
    pts = d.pts;
    mux.unlock();
    return d;
}
