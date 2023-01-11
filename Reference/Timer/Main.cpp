
#include "Timer.h"
#include <stdio.h>
#include <windows.h>

int main()
{
    HANDLE hEvent;
    Timer timer(&hEvent);
    bool bRunning = timer.start(1, 1000);

	while (bRunning) {
        /*
        SleepEx(
            INFINITE,     // Wait forever
            TRUE       // Put thread in an alertable state
        );
        */
        DWORD dwEvent = WaitForSingleObjectEx(
            hEvent,     // array of objects
            INFINITE,    // five-second wait
            TRUE
        );
        // The return value indicates which event is signaled

        switch (dwEvent)
        {
        // hEvents was signaled
        case WAIT_IO_COMPLETION:
            printf("WAIT_IO_COMPLETION\n");
            break;
        case WAIT_OBJECT_0:
            printf("WAIT_OBJECT_0\n");
            break;
        case WAIT_TIMEOUT:
            printf("WAIT_TIMEOUT\n");
            break;
        default:
            printf("Wait error: %d\n", GetLastError());
            ExitProcess(0);
        }

    }
};
