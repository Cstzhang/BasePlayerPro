//
//  IDecode.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IDecode.hpp"

//Data by the principal notify
void IDecode::Update(ZBData pkt)
{
    if(pkt.isAudio != isAudio)
    {
        return;
    }
    
    while (!isExit)
    {
        
        packsMutex.lock();
        
        //blocking
        if (packs.size() < maxList)
        {
            packs.push_back(pkt);
            packsMutex.unlock();
            break;
        }
        packsMutex.unlock();
        ZSleep(1);
    }
}

void  IDecode::Clear()
{
    
    packsMutex.lock();
    while (!packs.empty())
    {
        packs.front().Drop();
        packs.pop_front();
    }
    pts = 0;
    synPts = 0;
    packsMutex.unlock();
    
}

void IDecode::Main()
{
    while (!isExit)
    {
        
        
        if(IsPause())
        {
            ZSleep(2);
            continue;
        }
        
        packsMutex.lock();
        //Determine audio and video synchronization
        if(!isAudio && synPts > 0)
        {
            if (synPts < pts)
                //The time of the audio is less than the time of video
                //which means the audio is playing slower, and video stops and waits for the audio
            {
                packsMutex.unlock();
                ZSleep(1);
                continue;
                
            }
        }
        
        
        if (packs.empty())
        {
            packsMutex.unlock();
            ZSleep(1);//Releases the CPU currently occupied
            continue;
        }
        //Get packet
        ZBData pack = packs.front();//Take out the earliest data
        packs.pop_front();//Delete the data in the linked list after it is removed
        //Send data to the decoding thread
        if(this->SendPacket(pack))
        {
            while (!isExit)//It's going to come up on audio  Once the send Many times recv
            {
                //Acquisition of decoded data
                ZBData frame = RecvFrame();
                if (!frame.data) break;
                pts = frame.pts;
                //Send data to the observer
                this->Notify(frame);
            }
            
        }
        
        pack.Drop();//Clean up the data
        packsMutex.unlock();
        
        
    }
    
    
}

