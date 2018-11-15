//
//  IResample.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IResample_hpp
#define IResample_hpp

#include <stdio.h>
#include "IObserver.hpp"
#include "ZBParameters.hpp"

class IResample: public IObserver
{
public:
    virtual bool Open(ZBParameters in,ZBParameters out=ZBParameters()) = 0;
    virtual ZBData Resample(ZBData indata) = 0;
    virtual void Close() = 0;
    virtual void Update(ZBData data);
    int outChannels = 2;
    int outFormat = 1;
};


#endif /* IResample_hpp */
