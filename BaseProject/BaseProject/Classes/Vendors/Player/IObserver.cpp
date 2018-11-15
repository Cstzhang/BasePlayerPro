//
//  IObserver.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "IObserver.hpp"

//The main body function
void IObserver::AddObs(IObserver *obs)
{
    if(!obs)return;
    
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
    
}


//Notify all observers
void IObserver::Notify(ZBData data)
{
    mux.lock();
    for (int i = 0; i < obss.size(); ++i) {// obss.size() Don't use it when it's less efficient
        obss[i]->Update(data);
    }
    mux.unlock();
    
    
}
