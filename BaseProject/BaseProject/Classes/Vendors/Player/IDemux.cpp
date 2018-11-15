//
//  IDemux.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IDemux.hpp"

void IDemux::Main()
{
    while (!isExit)
    {
        if(IsPause())
        {
            ZSleep(2);
            continue;
        }
        ZBData d = Read();
        if (d.size > 0){
            Notify(d); //Process when no one receives it
        } else
        {
            ZSleep(2);
        }
    }
    
}
