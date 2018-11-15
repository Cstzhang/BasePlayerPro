//
//  IAudioPlay.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IAudioPlay_hpp
#define IAudioPlay_hpp

#include <stdio.h>
#include "IObserver.hpp"
#include "ZBParameters.hpp"
#include <list>

class IAudioPlay: public IObserver
{
public:
    //Receive resampling data  Block after buffer is full
    virtual void Update(ZBData data);
    //Get buffered data，If not, block
    virtual ZBData GetData();
    virtual bool StartPlay(ZBParameters out) = 0;
    virtual void Close() =  0;
    virtual void Clear();
    //Maximum buffer
    int maxFrame = 100; //100 fame
    long long pts = 0;
protected:
    std::list<ZBData> frames;//The audio queue
    std::mutex framesMutes;
    
};


#endif /* IAudioPlay_hpp */
