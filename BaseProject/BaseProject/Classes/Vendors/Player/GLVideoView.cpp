//
//  GLVideoView.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "GLVideoView.hpp"
#include "ZBTexture.hpp"

void  GLVideoView::SetRender(void *win,void *mycall)
{
    view = win;
    call = mycall;
}
void  GLVideoView::Close()
{
    mux.lock();
    
    if(txt)
    {
        txt->Drop();
        txt = 0;
    }
    
    mux.unlock();
}

void  GLVideoView::Render(ZBData data)
{
    if(!view)return;
    if(!txt)
    {
        //创建 texture
        txt = ZBTexture::Create();
        
        // 设置shader显示比例
        if (height == -10.0f) {
            float temp = 2 * width * (float)data.height / (float)data.width;
            height = (1.0f - temp * screenRatio);
        } else {
            height = 1.0f - 2 * height;
        }
        width = -1.0f + 2 * width;
        txt->SetShaderVertex(width, height);
        txt->Init(view,call, static_cast<ZTextureType>(data.format));
    }
    txt->Draw(data.datas,data.width,data.height);
    
}

void GLVideoView::SetShaderVertex(float widthRatioForScreen, float heightRationForScreen,float screenRatio) {
    mux.lock();
    // 设置宽高
    width = widthRatioForScreen;
    height = heightRationForScreen;
    this->screenRatio = screenRatio;
    
    mux.unlock();
}
