//
// Created by bigfish on 2018/10/12.
//

#ifndef ZPLAY_IDECODE_H
#define ZPLAY_IDECODE_H


//解码接口，支持硬解码

#include "ZParameter.h"
#include "IObserver.h"
#include <list>

class IDecode:public IObserver
{
public:


    //打开解码器
    virtual bool Open(ZParameter para, bool isHard= false) = 0;

    //future模型 多线程并发处理 发送数据到线程解码
    virtual bool SendPacket(ZData pkt) = 0;//立刻返回

    //从线程中获取解码结果 再次调用会复用上次空间 线程不安全
    virtual ZData RecvFrame()=0;

    virtual  void  Clear();

    //由主体notify的数据 阻塞的
    virtual void Update(ZData pkt);

    virtual void Close() = 0;

    bool isAudio = false;

    //最大队列缓冲
    int maxList  = 100; //100帧

    //同步时间,再次打开文件要清理
    long long synPts = 0;
    //当前已解码位置
    long long pts = 0;

protected:
    virtual void Main();//线程入口函数

    //读取缓冲
    std::list<ZData> packs;//链表队列

    std::mutex packsMutex;//读取队列的互斥变量


};


#endif //ZPLAY_IDECODE_H
