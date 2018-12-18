//
// Created by bigfish on 2018/10/16.
//

#ifndef ZPLAY_IVIDEOVIEW_H
#define ZPLAY_IVIDEOVIEW_H


#include "ZData.h"
#include "IObserver.h"

class IVideoView:public IObserver
{
public:
    //设置显示窗口
    
    virtual void SetRender(void *win,void *randerCall) = 0;
    virtual void Render(ZData data) = 0;
    virtual void Update(ZData data);
    virtual void Close() = 0;

};


#endif //ZPLAY_IVIDEOVIEW_H
