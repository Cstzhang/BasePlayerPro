//
//  IDemux.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IDemux_hpp
#define IDemux_hpp

#include <stdio.h>
#include "ZBData.hpp"
#include "IObserver.hpp"
#include "ZBParameters.hpp"
//解封装接口
class IDemux:public IObserver{
    
public:
    //Open files or stream media rtmp http rtsp
    virtual bool Open(const char *url) = 0;
    //seek pos = 0.0 ~ 1.0
    virtual bool Seek(double pos) = 0;
    //Read the video parameter
    virtual ZBParameters GetVPara() = 0;
    //Read audio parameters
    virtual ZBParameters GetAPara() = 0;
    
    virtual void Close() = 0;
    
    //Reads a frame of data, which is cleaned by the caller
    virtual ZBData Read() = 0;
    
    //Total time (ms)
    long long totalMs = 0;
    
protected:
    virtual void Main();
    
    
    
};




#endif /* IDemux_hpp */
