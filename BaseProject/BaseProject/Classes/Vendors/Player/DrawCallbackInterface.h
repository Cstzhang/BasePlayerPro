//
//  DrawCallbackInterface.h
//  BaseProject
//
//  Created by bigfish on 2018/11/16.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef DrawCallbackInterface_h
#define DrawCallbackInterface_h

enum ZTextureType
{
    ZTEXTURE_YUV420P = 0,    // Y4  U1  V1
    ZTEXTURE_YUVJ420P  = 12,    //软解码 虚拟机
    ZTEXTURE_NV12    = 23,   // Y4  UV1
    ZTEXTURE_NV21    = 24,    // Y4  VU1
    
};

enum ZShaderType
{
    ZSHADER_YUV420P = 0,    //软解码 虚拟机
    ZSHADER_YUVJ420P  = 12,    //软解码 虚拟机
    ZSHADER_NV12    = 23,   //手机
    ZSHADER_NV21    = 24,
    
};


//typedef void(*drawInterface)(void * caller ,ZTextureType type, int width, int height, unsigned char *data[]);
//
//typedef void(*initShaderInterface)(void * caller ,ZShaderType type);


typedef void(*interface) (void* caller, void* parameter);
typedef void(*drawInterface)(void* caller, void* data, unsigned int width,unsigned int height);
typedef void(*drawTest)(void* caller, unsigned int index, int width, int height, unsigned char* buf[], bool isa);

#endif /* DrawCallbackInterface_h */
