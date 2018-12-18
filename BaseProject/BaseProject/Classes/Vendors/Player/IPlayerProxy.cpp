//
// Created by bigfish on 2018/10/26.
//

#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"

void IPlayerProxy::Init(void *vm)
{
    mux.lock();
    if(vm)
    {
        FFPlayerBuilder::InitHard(vm);
    }

    if (!player)
    {
        player = FFPlayerBuilder::Get()->BuilderPlayer();
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
void IPlayerProxy::BindView(void *win,void *randerCall)
{
    mux.lock();
    if (player)
    {
        player->BindView(win,randerCall);
    }
    mux.unlock();
}



