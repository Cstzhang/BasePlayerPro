//
// Created by bigfish on 2018/10/16.
//

#ifndef ZPLAY_GLVIDEOVIEW_H
#define ZPLAY_GLVIDEOVIEW_H

#include "ZData.h"
#include "IVideoView.h"

class ZTexture;

class GLVideoView:public IVideoView
{
public:
    virtual void SetRender(void *win,void *randerCall);
    virtual void Render(ZData data);
    virtual void Close();
protected:
    //显示窗体
    void *view = 0;
    //回调
    void *rCall = 0;
    void *sCall = 0;
    
    ZTexture *txt = 0;
    std::mutex mux;
};


#endif //ZPLAY_GLVIDEOVIEW_H
