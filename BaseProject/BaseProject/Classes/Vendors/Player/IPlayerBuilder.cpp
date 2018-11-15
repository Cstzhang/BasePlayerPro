//
//  IPlayerBuilder.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IPlayerBuilder.hpp"
#include "IVideoView.hpp"
#include "IResample.hpp"
#include "IDecode.hpp"
#include "IAudioPlay.hpp"
#include "IDemux.hpp"

IPlayer  *IPlayerBuilder::BuilderPlayer(unsigned char index)
{
    IPlayer *play = CreatePlayer(index);
    //解封装
    IDemux *de = CreateDemux();
    //视频解码
    IDecode *vdecode =  CreateDecode();
    //音频解码
    IDecode *adecode =  CreateDecode();
    //解码器观察解封装
    de->AddObs(vdecode);
    de->AddObs(adecode);
    //显示观察视频解码器
    IVideoView *view = CreateVideoView();
    vdecode->AddObs(view);
    //重采样观察音频解码器
    IResample *resample = CreateResample();
    adecode->AddObs(resample);
    //音频播放观察重采样
    IAudioPlay *audioPlay = CreateAudioPlay();
    resample->AddObs(audioPlay);
    
    play->demux = de;
    play->adecode = adecode;
    play->vdecode = vdecode;
    play->videoView = view;
    play->resample = resample;
    play->audioPlay = audioPlay;
    
    return play;
}
