//
//  IObserver.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef IObserver_hpp
#define IObserver_hpp

#include <stdio.h>
#include "ZBData.hpp"
#include "ZBThread.hpp"
#include <vector>
#include <mutex>

//Observer and subject
class IObserver:public ZBThread
{
    
public:
    //The observer receives the data function
    virtual void Update(ZBData data) {}
    
    //Body functions add observers (thread-safe)
    void AddObs(IObserver *obs);
    
    //Notify all observers
    void Notify(ZBData data);
    
protected:
    std::vector<IObserver *>obss;
    std::mutex mux;
    
};

#endif /* IObserver_hpp */
