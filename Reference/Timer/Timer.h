#pragma once

#define _SECOND 10000000
#include <windows.h>

class Timer {
private:
    HANDLE hTimer;
    HANDLE* phEvent;
    bool bRunning;

public:
    Timer(HANDLE* phEvent);
    virtual ~Timer();

    virtual void initialize();
    virtual void finalize();

    void timesUp();

    bool start(int dueTime, int interval);
    void stop();

};