//
//  ZBResample.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "ZBResample.hpp"
#include "ZBLog.hpp"

extern "C"
{
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

void ZBResample::Close()
{
    mux.lock();
    if(actx)
    {
        swr_free(&actx);
    }
    mux.unlock();
}

bool ZBResample::Open(ZBParameters in,ZBParameters out)
{
    Close();
    mux.lock();
    //Audio resampling context initialization
    actx = swr_alloc();
    actx = swr_alloc_set_opts(actx,
                              av_get_default_channel_layout(out.channels),
                              AV_SAMPLE_FMT_S16,out.sample_rate,
                              av_get_default_channel_layout(in.para->channels),
                              (AVSampleFormat)in.para->format,in.para->sample_rate,
                              0,0 );
    
    int re = swr_init(actx);
    if(re != 0)
    {
        mux.unlock();
        ZLOGE("swr_init failed!");
        return false;
    }
    else
    {
        ZLOGI("swr_init success!");
    }
    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;
    mux.unlock();
    return true;
}

ZBData ZBResample::Resample(ZBData indata)
{
    if(indata.size<=0 || !indata.data) return ZBData();
    mux.lock();
    if(!actx)
    {
        mux.unlock();
        return ZBData();
    }
    
    //ZLOGI("indata size is %d",indata.size);
    AVFrame *frame = (AVFrame *)indata.data;
    
    //Allocation of output space
    ZBData out;
    int outsize = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if(outsize <=0)return ZBData();
    out.Alloc(outsize);
    uint8_t *outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx,outArr,frame->nb_samples,(const uint8_t **)frame->data,frame->nb_samples);
    if(len<=0)
    {
        mux.unlock();
        out.Drop();
        return ZBData();
    }
    out.pts = indata.pts;
    //    ZLOGI("swr_convert success = %d",len);
    mux.unlock();
    return out;
}
