
#include "Thread.h"

DWORD WINAPI _ThreadProc(LPVOID lpParam)
{
    Thread* pThread = (Thread*)lpParam;
    pThread->run();
    return 1;
}

Thread::Thread(EventQueue* pEventQueue, HANDLE* pHEvent) :
	pEventQueue(pEventQueue), pHEvent(pHEvent),
	bRunning(FALSE), hThread(NULL), dwThreadID(0)
{}

Thread::~Thread() {}

void Thread::initialize() {
    this->hThread = CreateThread(
        NULL,         // default security attributes
        0,            // default stack size
        (LPTHREAD_START_ROUTINE)_ThreadProc,
        this,         // no thread function arguments
        0,            // default creation flags
        &this->dwThreadID // receive thread identifier
    );

    if (hThread == NULL)
    {
        printf("CreateThread error: %d\n", GetLastError());
    }
	this->bRunning = TRUE;
}

void Thread::finalize() {}

void Thread::processAEvent() {
	unsigned value = this->pEventQueue->pop();
	if (value == 0) {
		this->start(1, 100);
	}
}

void Thread::run() {
	while (this->bRunning) {
		DWORD dwResult;
		dwResult = WaitForSingleObjectEx(
			*this->pHEvent,	// array of objects
			INFINITE,		// wait forever
			TRUE
		);
		switch (dwResult) {
		case WAIT_IO_COMPLETION:
			printf("WAIT_IO_COMPLETION\n");
			break;
		case WAIT_OBJECT_0:
			// hEvent was signaled
			printf("WAIT_OBJECT_0\n");
			this->processAEvent();
			break;
		case WAIT_TIMEOUT:
			printf("Wait timed out.\n");
			break;
		default:
			// Return value is invalid.
			printf("Wait error: %d\n", GetLastError());
			ExitProcess(0);
		}
		Sleep(10);
	}
}

bool Thread::start(int dueTime, int interval) {
	return FALSE;
}
void Thread::stop() {}