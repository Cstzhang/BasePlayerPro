//
//  ZBPlayerBuilder.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBPlayerBuilder_hpp
#define ZBPlayerBuilder_hpp

#include <stdio.h>
#include "IPlayerBuilder.hpp"

class ZBPlayerBuilder:public IPlayerBuilder
{
public:
    static void InitHard(void *vm);
    static ZBPlayerBuilder *Get()
    {
        static ZBPlayerBuilder ff;
        return  &ff;
    }
    
protected:
    ZBPlayerBuilder(){};
    virtual IDemux     *CreateDemux();
    virtual IDecode    *CreateDecode();
    virtual IResample  *CreateResample();
    virtual IVideoView *CreateVideoView();
    virtual IAudioPlay *CreateAudioPlay();
    virtual IPlayer    *CreatePlayer(unsigned char index=0);
    
    
};

#endif /* ZBPlayerBuilder_hpp */
