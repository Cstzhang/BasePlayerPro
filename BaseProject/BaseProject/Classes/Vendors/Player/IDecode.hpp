//
//  IDecode.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IDecode_hpp
#define IDecode_hpp

#include <stdio.h>
#include "ZBParameters.hpp"
#include "IObserver.hpp"
#include <list>

//Decoding interface, support hard decoding
class IDecode:public IObserver
{
public:
    
    
    //Turn on the decoder
    virtual bool Open(ZBParameters para, bool isHard= false) = 0;
    
    //Future model multithreading concurrent processing send data to the thread decoding
    virtual bool SendPacket(ZBData pkt) = 0;//Immediately return
    
    //Retrieving the decoding results from the thread again calls to reuse the last spatial thread unsafe
    virtual ZBData RecvFrame()=0;
    
    virtual  void  Clear();
    
    //Data by the principal notify blocking
    virtual void Update(ZBData pkt);
    
    virtual void Close() = 0;
    
    bool isAudio = false;
    
    //Maximum queue buffer
    int maxList  = 100; //100 frame
    
    //Sync time, open the file again to clean up
    long long synPts = 0;
    //Current decoded position
    long long pts = 0;
    
protected:
    virtual void Main();
    
    //Reads a buffer
    std::list<ZBData> packs;//List  queue
    
    std::mutex packsMutex;//Mutex of the queue
    
    
};



#endif /* IDecode_hpp */
