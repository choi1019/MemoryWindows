#pragma once

#include <windows.h>
#include "EventQueue.h"

class Thread {

private:
	HANDLE *pHEvent;
	EventQueue *pEventQueue;

	bool bRunning;
	HANDLE hThread;
	DWORD dwThreadID;

	void processAEvent();

public:
	Thread(EventQueue* pEventQueue, HANDLE* pHEvent);
	virtual ~Thread();
	virtual void initialize();
	virtual void finalize();

	virtual void run();
	virtual void stop();
};

