//
//  IAudioPlay.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IAudioPlay.hpp"

void IAudioPlay::Clear()
{
    framesMutes.lock();
    
    while (!frames.empty())
    {
        frames.front().Drop();
        frames.pop_front();
        
    }
    framesMutes.unlock();
}
ZBData IAudioPlay::GetData()
{
    ZBData d;
    isRunning = true;
    while(!isExit)
    {
        if(IsPause())
        {
            ZSleep(2);
            continue;
        }
        framesMutes.lock();
        if(!frames.empty())
        {
            //Data return
            d = frames.front();
            frames.pop_front();
            framesMutes.unlock();
            pts = d.pts;
            return  d;
        }
        
        framesMutes.unlock();
        ZSleep(1);
    }
    
    isRunning = false;
    //Unobtained data
    return d;
}



void IAudioPlay::Update(ZBData data)
{
    // ZLOGI("IAudioPlay::Update %d",data.size);
    //Push into the buffer queue
    if(data.size <= 0 || !data.data)return;
    while(!isExit)
    {
        framesMutes.lock();
        if (frames.size() > maxFrame)
        {
            framesMutes.unlock();
            ZSleep(1);
            continue;
        }
        frames.push_back(data);
        framesMutes.unlock();
        break;
    }
    
    
}
