//
//  ZBData.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBData_hpp
#define ZBData_hpp

#include <stdio.h>


enum ZBDataType
{
    AVPACKET_TYPE = 0,
    UCHAR_TYPE = 1
};

struct ZBData
{
    int type = 0;
    int pts = 0;
    unsigned char *data = 0;
    unsigned char *datas[8] = {0};//Decoded data
    int size = 0;
    bool isAudio = false;
    int width = 0;
    int height = 0;
    int format = 0;
    //Allocate space
    bool Alloc(int size, const char *d=0);
    //Clean up
    void Drop();
    
};



#endif /* ZBData_hpp */
