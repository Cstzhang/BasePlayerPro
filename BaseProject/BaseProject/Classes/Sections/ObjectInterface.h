//
//  ObjectInterface.h
//  FFmpeg402
//
//  Created by 廖石泉 on 2018/8/30.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ObjectInterface_h
#define ObjectInterface_h

typedef void(*interface) (void* caller, void* parameter);
typedef void(*drawInterface)(void* caller, void* data, unsigned int width,unsigned int height);
typedef void(*drawTest)(void* caller, unsigned int index, int width, int height, unsigned char* buf[], bool isa);

#endif /* ObjectInterface_h */
