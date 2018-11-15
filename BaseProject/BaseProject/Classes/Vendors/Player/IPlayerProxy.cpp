//
//  IPlayerProxy.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IPlayerProxy.hpp"
#include "ZBPlayerBuilder.hpp"

void IPlayerProxy::Init(void *vm)
{
    mux.lock();
    if(vm)
    {
        ZBPlayerBuilder::InitHard(vm);
    }
    
    if (!player)
    {
        player = ZBPlayerBuilder::Get()->BuilderPlayer();
    }
    mux.unlock();
}


void  IPlayerProxy::Close()
{
    mux.lock();
    if (player)
    {
        player->Close();
    }
    mux.unlock();
}

double  IPlayerProxy::PlayPos()
{
    double  pos = 0.0;
    mux.lock();
    if (player)
    {
        pos = player->PlayPos();
    }
    mux.unlock();
    return  pos;
}
bool IPlayerProxy::Seek(double pos)
{
    bool re = false;
    mux.lock();
    if (player)
    {
        player->Seek(pos);
    }
    mux.unlock();
    return re;
}
bool IPlayerProxy::IsPause()
{
    bool re = false;
    mux.lock();
    if (player)
    {
        re = player->IsPause();
    }
    mux.unlock();
    return re;
    
}


void IPlayerProxy::SetPause(bool isP)
{
    mux.lock();
    if (player)
    {
        player->SetPause(isP);
    }
    mux.unlock();
    
}

bool IPlayerProxy::Open(const char *path)
{
    bool re = false;
    mux.lock();
    if (player)
    {
        player->isHardDecode = isHardDecode;
        re = player->Open(path);
    }
    mux.unlock();
    return re;
}

bool IPlayerProxy::Start()
{
    bool re = false;
    mux.lock();
    if (player)
    {
        re = player->Start();
    }
    mux.unlock();
    return re;
}
void IPlayerProxy::InitView(void *win,void *call)
{
    mux.lock();
    if (player)
    {
        player->InitView(win,call);
    }
    mux.unlock();
}
void IPlayerProxy::SetShaderVertex(float widthRatioForScreen, float heightRationForScreen, float screenRatio) {
    mux.lock();
    if (player)
        player->SetShaderVertex(widthRatioForScreen, heightRationForScreen, screenRatio);
    mux.unlock();
}
