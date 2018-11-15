//
//  ZBLog.hpp
//  BaseProject
//
//  Created by bigfish on 2018/11/14.
//  Copyright © 2018 bigfish. All rights reserved.
//

#ifndef ZBLog_hpp
#define ZBLog_hpp

#include <stdio.h>

class ZBLog {
    
};

#ifdef ANDROID
//#include <android/log.h>
//#define ZLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"ZPlay",__VA_ARGS__)
//#define ZLOGI(...) __android_log_print(ANDROID_LOG_INFO,"ZPlay",__VA_ARGS__)
//#define ZLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ZPlay",__VA_ARGS__)
#else
//其他平台

#define ZLOGE(...) printf(__VA_ARGS__)
#define ZLOGI(...) printf(__VA_ARGS__)

#endif


#endif /* ZBLog_hpp */
