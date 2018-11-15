//
//  IPlayer.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IPlayer_hpp
#define IPlayer_hpp

#include <stdio.h>
#include <mutex>
#include "ZBThread.hpp"
#include "ZBParameters.hpp"



class  IDemux;
class  IDecode;
class  IResample;
class  IVideoView;
class  IAudioPlay;

class IPlayer: public ZBThread
{
public:
    static IPlayer *Get(unsigned char index=0);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void InitView(void *win,void *call);
    //Gets current playback progress 0.0~1.0
    virtual double PlayPos();
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);
    virtual void SetShaderVertex(float widthRatioForScreen, float heightRationForScreen, float screenRatio);
    
    //Is it hard decoding
    bool isHardDecode = false;//No open by default
    
    IDemux     *demux     = 0;
    IDecode    *vdecode   = 0;
    IDecode    *adecode   = 0;
    IResample  *resample  = 0;
    IVideoView *videoView = 0;
    IAudioPlay *audioPlay = 0;
    //Audio output parameter
    ZBParameters outPara;
    
protected:
    
    void Main(); //Used to synchronize audio and video
    std::mutex mux;
    IPlayer(){};
    
};



#endif /* IPlayer_hpp */
