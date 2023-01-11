
#include "Thread.h"

int main()
{
	HANDLE hEvent;
	EventQueue* pEventQueue = new EventQueue(&hEvent);

	Thread* pThread = new Thread(pEventQueue, &hEvent);
	pThread->initialize();

	for (int i=0; i<10000; i++) {
		pEventQueue->push(i);
		Sleep(100);
	}
};
