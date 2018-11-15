//
//  GLVideoView.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef GLVideoView_hpp
#define GLVideoView_hpp

#include <stdio.h>
#include "ZBData.hpp"
#include "IVideoView.hpp"

class ZBTexture;

class GLVideoView:public IVideoView
{
public:
    virtual void SetRender(void *win,void *call);
    virtual void Render(ZBData data);
    virtual void Close();
    void SetShaderVertex(float widthRatioForScreen, float heightRationForScreen,float screenRatio) override;
protected:
    //显示窗体
    void *view = 0;
    void *call = 0;
    ZBTexture *txt = 0;
    std::mutex mux;
};


#endif /* GLVideoView_hpp */
