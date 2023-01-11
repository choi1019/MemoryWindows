#pragma once

#define _SECOND 10000000
#include <windows.h>
#include "Thread.h"

class Timer: public Thread {
private:
    HANDLE hTimer;
    HANDLE* phEvent;
    bool bRunning;

public:
    Timer(EventQueue* pEventQueue, HANDLE* phEvent);
    virtual ~Timer();

    virtual void initialize();
    virtual void finalize();

    void timesUp();

    virtual bool start(int dueTime, int interval);
    virtual void stop();

};