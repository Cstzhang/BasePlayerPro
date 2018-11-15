//
//  IPlayerProxy.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IPlayerProxy_hpp
#define IPlayerProxy_hpp

#include <stdio.h>
#include <mutex>
#include "IPlayer.hpp"

class IPlayerProxy: public  IPlayer
{
public:
    static IPlayerProxy *Get()
    {
        static IPlayerProxy px;
        return &px;
        
    }
    
    void Init(void *vm = 0);
    virtual bool Open(const char *path) override;
    virtual void Close() override;
    virtual bool Start() override;
    virtual void InitView(void *win,void *call) override;
    virtual double PlayPos() override;
    virtual bool Seek(double pos) override;
    virtual void SetPause(bool isP) override;
    virtual bool IsPause() override;
    void SetShaderVertex(float widthRatioForScreen, float heightRationForScreen, float screenRatio) override;
protected:
    IPlayerProxy(){};
    IPlayer *player = 0;
    std::mutex mux;
};
#endif /* IPlayerProxy_hpp */
