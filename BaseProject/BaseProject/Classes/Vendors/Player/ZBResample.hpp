//
//  ZBResample.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBResample_hpp
#define ZBResample_hpp

#include <stdio.h>
#include "IResample.hpp"
#include <mutex>

struct SwrContext;
class ZBResample: public IResample
{
public:
    virtual bool Open(ZBParameters in,ZBParameters out=ZBParameters());
    virtual void Close();
    virtual ZBData Resample(ZBData indata);
protected:
    SwrContext *actx = 0;
    std::mutex mux;
};


#endif /* ZBResample_hpp */
