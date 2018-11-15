//
//  IVideoView.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IVideoView_hpp
#define IVideoView_hpp

#include <stdio.h>
#include "ZBData.hpp"
#include "IObserver.hpp"



class IVideoView:public IObserver
{
public:
    //Set display window
    virtual void SetRender(void *win,void *call) = 0;
    virtual void Render(ZBData data) = 0;
    virtual void Update(ZBData data);
    virtual void Close() = 0;
    virtual void SetShaderVertex(float widthRatioForScreen, float heightRationForScreen,float screenRatio){}
protected:
    float width = 1.0f; // 屏幕宽 -- 用来在shader中设置vertex属性
    float height = 1.0f; // 屏幕高 -- 用来在shader中设置vertex属性
    float screenRatio = 1.0f; // 屏幕高比例
};
#endif /* IVideoView_hpp */
