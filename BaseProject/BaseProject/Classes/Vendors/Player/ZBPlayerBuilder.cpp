//
//  ZBPlayerBuilder.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "ZBPlayerBuilder.hpp"
#include "ZBDemux.hpp"
#include "ZBDecode.hpp"
#include "ZBResample.hpp"
#include "GLVideoView.hpp"
//#include "SLAudioPlay.hpp"

IDemux     *ZBPlayerBuilder::CreateDemux()
{
    IDemux *ff = new ZBDemux();
    return ff;
}
IDecode    *ZBPlayerBuilder::CreateDecode()
{
    IDecode *ff = new ZBDecode();
    return ff;
}
IResample  *ZBPlayerBuilder::CreateResample()
{
    IResample *ff = new ZBResample();
    return ff;
}
IVideoView *ZBPlayerBuilder::CreateVideoView()
{
    IVideoView *ff = new GLVideoView();
    return ff;
}
IAudioPlay *ZBPlayerBuilder::CreateAudioPlay()
{
//    IAudioPlay *ff = new SLAudioPlay();
//    return ff;
      return NULL;
}
IPlayer    *ZBPlayerBuilder::CreatePlayer(unsigned char index)
{
    return IPlayer::Get(index);
}

void ZBPlayerBuilder::InitHard(void *vm)
{
    ZBDecode::InitHard(vm);
}
