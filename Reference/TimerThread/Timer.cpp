#pragma once
#include <stdio.h>
#include <windows.h>

#include "Timer.h"

VOID CALLBACK TimerAPCProc(
    LPVOID lpArg,              // Data value
    DWORD dwTimerLowValue,     // Timer low value
    DWORD dwTimerHighValue)    // Timer high value
{
    // Formal parameters not used in this example.
    UNREFERENCED_PARAMETER(dwTimerLowValue);
    UNREFERENCED_PARAMETER(dwTimerHighValue);

    Timer *pTimer = (Timer *)lpArg;
    pTimer->timesUp();
}

Timer::Timer(EventQueue* pEventQueue, HANDLE* phEvent) : Thread(pEventQueue, phEvent),
    phEvent(phEvent), hTimer(NULL), bRunning(FALSE)
   {
    /*
    *this->phEvent = CreateEvent(
        NULL,   // default security attributes
        FALSE,  // auto-reset event object
        FALSE,  // initial state is nonsignaled
        NULL  // unnamed object
    );
    if (*this->phEvent == NULL)
    { 
        printf("CreateEvent error: %d\n", GetLastError());
        ExitProcess(0);
    }
    */
    this->hTimer = CreateWaitableTimer(
        NULL,                   // Default security attributes
        FALSE,                  // Create auto-reset timer
        TEXT("MyTimer")         // Name of waitable timer
    );
    if (this->hTimer == NULL) {
        printf("CreateWaitableTimer failed with error %d\n", GetLastError());
        ExitProcess(0);
    }
}

Timer::~Timer() {
    CloseHandle(*this->phEvent);
    CloseHandle(this->hTimer);
}

void Timer::initialize() {
    Thread::initialize();
}
void Timer::finalize() {
    Thread::finalize();
}

bool Timer::start(int dueTime, int interval) {
    __int64 qwDueTime = (__int64)-dueTime * _SECOND;
    // Copy the relative time into a LARGE_INTEGER
    LARGE_INTEGER liDueTime;
    liDueTime.LowPart = (DWORD)(qwDueTime & 0xFFFFFFFF);
    liDueTime.HighPart = (LONG)(qwDueTime >> 32);

    bool bSuccess = SetWaitableTimer(
        this->hTimer,     // Handle to the timer object
        &liDueTime,       // When timer will become signaled
        interval,         // Periodic timer
        TimerAPCProc,     // Completion routine
        this,             // Argument to the completion routine
        FALSE             // Do not restore a suspended system
    );

    if (!bSuccess) {
        printf("SetWaitableTimer failed with error %d\n", GetLastError());
    }
    return bSuccess;
}

void Timer::timesUp() {
    printf("Timer::timesUp()\n");
    if (!SetEvent(*this->phEvent))
    {
        printf("SetEvent failed (%d)\n", GetLastError());
    }
}

void Timer::stop() {
    this->bRunning = FALSE;
}




