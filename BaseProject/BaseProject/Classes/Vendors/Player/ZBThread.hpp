//
//  ZBThread.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/15.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBThread_hpp
#define ZBThread_hpp

#include <stdio.h>


//sleep ms
void ZSleep(int mis);


//c++11 Threading library
class ZBThread
{
public:
    //Starting a thread
    virtual bool Start();
    
    //Stop threads safely by controlling isExit (not necessarily successful)
    virtual void Stop();
    
    //Entry main function
    virtual void Main(){}
    
    virtual  bool IsPause()
    {
        isPausing = isPause;
        return isPause;
    }
    
    virtual void SetPause(bool isP);
    
protected:
    bool isExit = false;
    bool isRunning = false;
    bool isPause = false;
    bool isPausing = false;
    
    
private:
    void ThreadMain();
    
    
};


#endif /* ZBThread_hpp */
