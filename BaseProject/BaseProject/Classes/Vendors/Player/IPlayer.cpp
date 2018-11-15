//
//  IPlayer.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IPlayer.hpp"
#include "IDemux.hpp"
#include "IDecode.hpp"
#include "IResample.hpp"
#include "IAudioPlay.hpp"
#include "IVideoView.hpp"
#include "ZBLog.hpp"

IPlayer *IPlayer::Get(unsigned char index)
{
    static IPlayer p[256];
    return  &p[index];
}

//Used to synchronize audio and video
void IPlayer::Main()
{
    
    while (!isExit)
    {
        mux.lock();
        if(!audioPlay || !vdecode)
        {
            mux.unlock();
            ZSleep(2);
            continue;
            
        }
        //synchronize
        //Get PTS for audio
        long long apts = audioPlay->pts;
        // ZLOGE(" apts %d",apts);
        vdecode->synPts = apts;
        
        mux.unlock();
        ZSleep(200);
    }
    
}
void IPlayer::Close()
{
    mux.lock();
    //1 Close the main thread before cleaning up the observer
    
    //Thread synchronization
    ZBThread::Stop();
    //decapsulation
    if(demux)
    {
        demux->Stop();
    }
    
    //decode
    if(vdecode)
    {
        vdecode->Stop();
    }
    
    if(adecode)
    {
        adecode->Stop();
    }
    
    if(audioPlay)
    {
        audioPlay->Stop();
    }
    
    
    //2 Clear buffer queue
    if(vdecode)
    {
        vdecode->Clear();
    }
    
    if(adecode)
    {
        adecode->Clear();
    }
    
    if(audioPlay)
    {
        audioPlay->Clear();
    }
    
    
    //3 Clean up resources
    if(audioPlay)
    {
        audioPlay->Close();
    }
    
    if(videoView)
    {
        videoView->Close();
    }
    
    if(vdecode)
    {
        vdecode->Close();
    }
    
    if(adecode)
    {
        adecode->Close();
    }
    
    if(demux)
    {
        demux->Close();
    }
    
    mux.unlock();
}

double IPlayer::PlayPos()
{
    
    double pos = 0.0;
    mux.lock();
    long long total = 0;
    if(demux)
        total = demux->totalMs;
    if(total>0)
    {
        if(vdecode)
        {
            pos = (double)vdecode->pts/(double)total;
        }
    }
    mux.unlock();
    //    ZLOGE("pos   %lld",vdecode->pts/total);
    return pos;
}

void IPlayer::SetPause(bool isP)
{
    mux.lock();
    ZBThread::SetPause(isP);
    if(demux)
    {
        demux->SetPause(isP);
    }
    if(vdecode)
    {
        vdecode->SetPause(isP);
    }
    if(adecode)
    {
        adecode->SetPause(isP);
    }
    if(audioPlay)
    {
        audioPlay->SetPause(isP);
    }
    mux.unlock();
    
}

bool IPlayer::Seek(double pos)
{
    bool re = false;
    if(!demux) return false;
    
    //Pause all threads
    SetPause(true);
    mux.lock();
    //Clean up the buffer
    //2 Clear buffer queue
    if(vdecode)
        vdecode->Clear(); //Clear the buffer queue, clear the ffmpeg buffer
    if(adecode)
        adecode->Clear();
    if(audioPlay)
        audioPlay->Clear();
    
    
    re = demux->Seek(pos); //seek Skip to keyframe
    if(!vdecode)
    {
        mux.unlock();
        SetPause(false);
        return re;
    }
    //Decoding to the actual frame to be displayed
    int seekPts = pos*demux->totalMs;
    while(!isExit)
    {
        ZBData pkt = demux->Read();
        if(pkt.size<=0)break;
        if(pkt.isAudio)
        {
            if(pkt.pts < seekPts)
            {
                pkt.Drop();
                continue;
            }
            //Write buffer queue
            demux->Notify(pkt);
            continue;
        }
        
        //Decode the data before the frame that needs to be displayed
        vdecode->SendPacket(pkt);
        pkt.Drop();
        ZBData data = vdecode->RecvFrame();
        if(data.size <=0)
        {
            continue;
        }
        if(data.pts >= seekPts)
        {
            //vdecode->Notify(data);
            break;
        }
    }
    mux.unlock();
    
    SetPause(false);
    return re;
    
}

bool IPlayer::Open(const char *path)
{
    Close();
    mux.lock();
    //decapsulation
    if(!demux || !demux->Open(path))
    {
        mux.unlock();
        ZLOGE("demux->Open failed! %s",path);
        return false;
    }
    //Decoding and decoding may not require unsealing after the original data
    if(!vdecode || !vdecode->Open(demux->GetVPara(),isHardDecode))
    {
        ZLOGE("vdecode->Open failed! %s",path);
        //return false;
    }
    
    if(!adecode || !adecode->Open(demux->GetAPara()))
    {
        ZLOGE("vdecode->Open failed! %s",path);
        //return false;
    }
    //Resampling may not require decoding or unsealing after installation can be played
    outPara = demux->GetAPara();
    //    if(outPara.sample_rate <=0)
    //    {
    
    //    }
    if(!resample || !resample->Open(demux->GetAPara(),outPara))
    {
        ZLOGE("resample->Open failed! %s",path);
    }
    mux.unlock();
    return true;
}


bool IPlayer::Start()
{
    mux.lock();
    
    if(audioPlay)
    {
        audioPlay->StartPlay(outPara);
    }
    if(vdecode)
    {
        vdecode->Start();
    }
    if (adecode)
    {
        adecode->Start();
    }
    
    if(!demux || !demux->Start())
    {
        mux.unlock();
        ZLOGE("demux->Start failed! ");
        return false;
    }
    //
    //    if(vdecode)
    //        vdecode->Start();
    //
    //
    //
    //    if(!demux || !demux->Start())
    //    {
    //        mux.unlock();
    //        ZLOGE("demux->Start failed!");
    //        return false;
    //    }
    //    if(adecode)
    //        adecode->Start();
    //    if(audioPlay)
    //        audioPlay->StartPlay(outPara);
    
    
    ZBThread::Start();
    
    mux.unlock();
    return true;
}

void IPlayer::InitView(void *win,void *call)
{
    if(videoView)
    {
        videoView->Close();
        videoView->SetRender(win,call);
    }
    
}

void IPlayer::SetShaderVertex(float widthRatioForScreen, float heightRationForScreen, float screenRatio) {
    mux.lock();
    videoView->SetShaderVertex(widthRatioForScreen, heightRationForScreen,screenRatio);
    mux.unlock();
}

