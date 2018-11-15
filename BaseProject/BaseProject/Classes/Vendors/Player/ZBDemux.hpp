//
//  ZBDemux.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBDemux_hpp
#define ZBDemux_hpp

#include <stdio.h>
#include "IDemux.hpp"
#include <mutex>

struct AVFormatContext;

class ZBDemux: public IDemux
{
    
public:
    //Open files or stream media rtmp http rtsp
    virtual bool Open(const char *url);
    //seek pos = 0.0 ~ 1.0
    virtual bool Seek(double pos);
    virtual void Close();
    //Read the video parameter
    virtual ZBParameters GetVPara();
    //Read audio parameters
    virtual ZBParameters GetAPara();
    
    //Reads a frame of data, which is cleaned by the caller
    virtual ZBData Read();
    //The constructor
    ZBDemux();
    
private:
    AVFormatContext *ic = 0; //The initializer takes effect without a parameter c++11
    std::mutex mux;
    int audioStream = 1;
    int videoStream = 0;
};


#endif /* ZBDemux_hpp */
