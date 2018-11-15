//
//  IPlayerBuilder.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IPlayerBuilder_hpp
#define IPlayerBuilder_hpp

#include <stdio.h>
#include "IPlayer.hpp"

class IPlayerBuilder
{
public:
    virtual IPlayer    *BuilderPlayer(unsigned char index=0);
protected:
    virtual IDemux     *CreateDemux()                     = 0;
    virtual IDecode    *CreateDecode()                    = 0;
    virtual IResample  *CreateResample()                  = 0;
    virtual IVideoView *CreateVideoView()                 = 0;
    virtual IAudioPlay *CreateAudioPlay()                 = 0;
    virtual IPlayer    *CreatePlayer(unsigned char index=0) = 0;
    
};

#endif /* IPlayerBuilder_hpp */
