//
//  ZBTexture.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBTexture_hpp
#define ZBTexture_hpp

#include <stdio.h>

enum ZTextureType
{
    ZTEXTURE_YUV420P = 0,    // Y4  U1  V1
    ZTEXTURE_YUVJ420P = 12,  // Y 4  u 1 v 1
    ZTEXTURE_NV12    = 25,   // Y4  UV1
    ZTEXTURE_NV21    = 26    // Y4  VU1
};


class ZBTexture
{
public:
    static ZBTexture *Create();
    virtual bool Init(void *win,void *call,ZTextureType type=ZTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char *data[],int width,int height) = 0;
    virtual void Drop() = 0;
    virtual void SetShaderVertex(float widthRatioForScreen, float heightRationForScreen) = 0;
    virtual ~ZBTexture(){};
    
protected:
    ZBTexture(){};
    
};


#endif /* ZBTexture_hpp */
