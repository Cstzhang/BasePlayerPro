//
// Created by bigfish on 2018/10/16.
//

#ifndef ZPLAY_ZSHADER_H
#define ZPLAY_ZSHADER_H

#include <mutex>
#include <iostream>
#include "DrawCallbackInterface.h"

class ZShader
{
    
    
public:
    //初始化
    virtual bool Init(ZShaderType type = ZSHADER_YUV420P);
    virtual void Close();
    //获取材质并映射到内存
    /*
     *index 材质第几层索引
     * */
    virtual void GetTexture(unsigned int index,int width,int height, unsigned char *buf,bool isa=false);
    virtual void Draw();

protected:
    unsigned int vsh = 0;
    unsigned int fsh = 0;
    unsigned int program = 0;
    unsigned int texts[100] = {0};
    std::mutex mux;

};


#endif //ZPLAY_ZSHADER_H
