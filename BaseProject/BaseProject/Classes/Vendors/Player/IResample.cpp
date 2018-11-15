//
//  IResample.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IResample.hpp"

void IResample::Update(ZBData data)
{
    ZBData d = this->Resample(data);
    //    ZLOGI("IResample::Update %d",d.size);
    if (d.size > 0)
    {
        this->Notify(d);
    }
}
