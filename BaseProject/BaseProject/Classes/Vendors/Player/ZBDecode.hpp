//
//  ZBDecode.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBDecode_hpp
#define ZBDecode_hpp

#include <stdio.h>
#include "ZBParameters.hpp"
#include "IDecode.hpp"
#include <mutex>

struct AVCodecContext;
struct AVFrame;
class ZBDecode:public IDecode

{
public:
    static void InitHard(void *vm);
    
    virtual bool Open(ZBParameters para,bool isHard=false);
    virtual void Close();
    //The future model sends data to the thread for decoding
    virtual bool SendPacket(ZBData pkt);
    
    //Get the decoding results from the thread, and a second call will reuse the last space, making the thread unsafe
    virtual ZBData RecvFrame();
    virtual  void  Clear();
protected:
    AVCodecContext *codec = 0;
    AVFrame *frame = 0;
    std::mutex mux;
};

#endif /* ZBDecode_hpp */
