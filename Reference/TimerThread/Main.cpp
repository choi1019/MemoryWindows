
#include "Timer.h"

int main()
{
	HANDLE hEvent;
	EventQueue* pEventQueue = new EventQueue(&hEvent);

	Timer* pTimer = new Timer(pEventQueue, &hEvent);
	pTimer->initialize();

	pTimer->start(1, 100);
	for (int i=0; i<10000; i++) {
		pEventQueue->push(i);
		Sleep(100);
	}
};
