//
//  ZBTexture.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "ZBTexture.hpp"
#include "ZBLog.hpp"
#include <mutex>
#include "ObjectInterface.h"
//#include "ZEGL.h"
//#include "ZBShader.h"

class CZTexture:public ZBTexture
{
public:
//    ZShader sh;
    void *win;
    drawTest call;
    ZTextureType type;
    std::mutex mux;
    unsigned char * buf[];
    
    virtual void Drop() override
    {
        mux.lock();
//        ZEGL::Get()->Close();
//        sh.Close();
        mux.unlock();
        delete this;
    }
    
    virtual bool Init(void *win,void *call,ZTextureType type) override
    {
        mux.lock();
//        ZEGL::Get()->Close();
//        sh.Close();
        this->type = type;
        if(!win)
        {
            mux.unlock();
            ZLOGE("ZTexture init failed win is NULL");
            return false;
        }
        this->win = win;
        this->call = (drawTest)call;
        //初始化EGL
//        if(!ZEGL::Get()->Init(win))
//        {
//            mux.unlock();
//            return false;
//        }
//
        
        //初始化shader
//        sh.Init((ZShaderType)type);
        mux.unlock();
        return true;
    }
    virtual void Draw(unsigned char *data[],int width,int height) override
    {
        mux.lock();
        buf[0] = data[0];
//        sh.GetTexture(0,width,height,data[0]);  // Y
        
        if(type == ZTEXTURE_YUV420P || type == ZTEXTURE_YUVJ420P)
        {
            buf[1] = data[1];
            buf[2] = data[2];
            // ZLOGI("===== ZTEXTURE_YUV420P ======");
//            sh.GetTexture(1,width/2,height/2,data[1]);  // U
//            sh.GetTexture(2,width/2,height/2,data[2]);  // V
        }
        else
        {
             buf[1] = data[1];
            //ZLOGI("===== ZTEXTURE_nv12 ======");
          //  sh.GetTexture(1,width/2,height/2,data[1], true);  // UV
        }
//        sh.Draw();
//        ZEGL::Get()->Draw();
            this->call(this->win,0,width,height,buf,false);
        mux.unlock();
    }
    void SetShaderVertex(float widthRatioForScreen, float heightRationForScreen) override {
        mux.lock();
        //        sh.SetShaderVertex(widthRatioForScreen, heightRationForScreen);
        mux.unlock();
    }
    
};


ZBTexture *ZBTexture::Create()
{
    return new CZTexture();
}
