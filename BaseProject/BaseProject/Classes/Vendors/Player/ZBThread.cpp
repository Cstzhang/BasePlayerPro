//
//  ZBThread.cpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#include "ZBThread.hpp"
#include <thread>
#include "ZBLog.hpp"

using namespace std;

void ZSleep(int mis)
{
    chrono::milliseconds du(mis);//ms
    this_thread::sleep_for(du);
}


void ZBThread::SetPause(bool isP)
{
    isPause = isP;
    //Wait 100 milliseconds
    for(int i = 0; i < 10; i++)
    {
        if(isPausing == isP)
        {
            break;
        }
        ZSleep(10);
    }
    
}

//Starting a thread
bool ZBThread::Start()
{
    
    isExit = false;
    isPause = false;
    
    thread th(&ZBThread::ThreadMain,this);
    
    th.detach();//Relinquish control of threads
    
    return true;
    
}

void ZBThread::ThreadMain()
{
    
    isRunning = true;
    Main();
    isRunning = false;
}


//Stop threads safely by controlling isExit (not necessarily successful)
void ZBThread::Stop()
{
    isExit = true;
    for (int i = 0; i < 200; ++i) {//200 milliseconds
        if(!isRunning){
            ZLOGI("Stop thread succeeded");
            return;
        }
        ZSleep(1);
    }
    ZLOGI("Stop thread timeout");
    
}
 
