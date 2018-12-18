//
// Created by bigfish on 2018/10/16.
//

#ifndef ZPLAY_ZTEXTURE_H
#define ZPLAY_ZTEXTURE_H
#include "ZShader.h"


class ZTexture
{
public:
    static ZTexture *Create();
    virtual bool Init(void *win,void *rCall,ZTextureType type=ZTEXTURE_YUV420P) = 0;
    virtual void Draw(unsigned char *data[],int width,int height) = 0;
    virtual void Drop() = 0;
    virtual ~ZTexture(){};
protected:
    ZTexture(){};

};


#endif //ZPLAY_ZTEXTURE_H
