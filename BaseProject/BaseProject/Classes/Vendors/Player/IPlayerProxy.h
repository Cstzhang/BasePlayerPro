//
// Created by bigfish on 2018/10/26.
//

#ifndef ZPLAY_IPLAYERPROXY_H
#define ZPLAY_IPLAYERPROXY_H


#include <mutex>
#include "IPlayer.h"
#include <mutex>
class IPlayerProxy: public  IPlayer
{
public:
    static IPlayerProxy *Get()
    {
        static IPlayerProxy px;
        return &px;

    }

    void Init(void *vm = 0);
    virtual bool Open(const char *path);
    virtual void Close();
    virtual bool Start();
    virtual void BindView(void *win,void *randerCall,void *shaderCall);
    virtual double PlayPos();
    virtual bool Seek(double pos);
    virtual void SetPause(bool isP);
    virtual bool IsPause();
protected:
    IPlayerProxy(){};
    IPlayer *player = 0;
    std::mutex mux;
};


#endif //ZPLAY_IPLAYERPROXY_H
