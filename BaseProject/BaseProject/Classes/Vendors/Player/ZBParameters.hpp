//
//  ZBParameters.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBParameters_hpp
#define ZBParameters_hpp

#include <stdio.h>

struct  AVCodecParameters;

class ZBParameters
{
public:
    AVCodecParameters *para = 0;
    int channels = 2; //The channels num
    int sample_rate = 44100;//The sample rate
    
};

#endif /* ZBParameters_hpp */
