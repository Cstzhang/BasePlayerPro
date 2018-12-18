//
// Created by bigfish on 2018/10/16.
//

#include "ZTexture.h"
#include "ZLog.h"
#include<iostream>
#include<thread>
using namespace std;

#ifdef __OBJC__
#include "ZPlayerView.h"
#endif



class CZTexture:public ZTexture
{
public:
    void *win;
//    drawInterface drawCall;
    drawTest drawCall;
//    initShaderInterface initShaderCall;
   // ZShader sh;
    ZTextureType type;
    std::mutex mux;
    unsigned char* buf[];
    
    virtual void Drop()
    {
        mux.lock();
        
       // sh.Close();
//        ZPlaye
        
        mux.unlock();
        delete this;
    }

    virtual bool Init(void *win,void *rCall,ZTextureType type)
    {
        mux.lock();
//        ZEGL::Get()->Close();
       // sh.Close();
//        std::thread::id tid = std::this_thread::get_id();
//        cout << "==============f id=" << tid << endl;
        this->type = type;
        if(!win)
        {
            mux.unlock();
            ZLOGE("ZTexture init failed win is NULL");
            return false;
        }
        //初始化EGL
//        if(!ZEGL::Get()->Init(win))
//        {
//            mux.unlock();
//            return false;
//        }

        this->win = win;
        this->drawCall = (drawTest)rCall;
//        this->initShaderCall =(initShaderInterface)sCall;
        //初始化shader
       // sh.Init((ZShaderType)type);
//        this->initShaderCall(this->win,(ZShaderType)type);
        mux.unlock();
        return true;
    }
    virtual void Draw(unsigned char *data[],int width,int height)
    {
        //  NSLog(@"%@",NSThread.currentThread);
//        std::thread::id tid = std::this_thread::get_id();
//        cout << "==============f id=" << tid << endl;
        mux.lock();
      //  sh.GetTexture(0,width,height,data[0]);  // Y
        buf[0] = data[0];
        
        if(type == ZTEXTURE_YUV420P || type ==ZTEXTURE_YUVJ420P)
        {
            buf[1] = data[1];
            buf[2] = data[2];
           // ZLOGI("===== ZTEXTURE_YUV420P ======");
        //    sh.GetTexture(1,width/2,height/2,data[1]);  // U
          //  sh.GetTexture(2,width/2,height/2,data[2]);  // V
        }
        else
        {
            buf[1] = data[1];
            //ZLOGI("===== ZTEXTURE_nv12 ======");
         //   sh.GetTexture(1,width/2,height/2,data[1], true);  // UV
        } 
        //sh.Draw();
//        ZEGL::Get()->Draw();
//        this->drawCall(this->win,this->type,width,height,buf);
          this->drawCall(this->win,0,width,height,buf,false);
        mux.unlock();
    }

};


ZTexture *ZTexture::Create()
{
    return new CZTexture();
}
