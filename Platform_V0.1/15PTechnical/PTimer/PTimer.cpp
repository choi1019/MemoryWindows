//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#include "PTimer.h"
#define _MILISECOND 10000

PTimer::PTimer(String componentName, __int64 initialDelay, DWORD period)
	: initialDelay(initialDelay), period(period),
	  PTask(componentName, Make_ClassInfo(PTIMER_ID, PTIMER), &hEvents[eTaskMsgQueue], &hEvents[eComponentMsgQueue])
{

	// Create a negative 64-bit integer that will be used to
	// signal the timer ? Miliseconds from now.
	__int64 qwDueTime;  // initial delay
	qwDueTime = -10000 * initialDelay;
	liDueTime.QuadPart = qwDueTime;

	hEvents[eTimer] = CreateWaitableTimer(
		NULL,	// Default security attributes.
		FALSE,	// Create auto-reset timer.
		NULL);	// Name of waitable timer.

	if (hEvents[eTimer] == NULL) {
		printf("CreateWaitableTimer() failed with Error %d", GetLastError());
	}
}

PTimer::~PTimer() {
	CloseHandle(hEvents[eTimer]);
}

void PTimer::initialize() {
	PTask::initialize();
}

void PTimer::finalize() {
	PTask::finalize();
}

void PTimer::startAComponent() {
	bool bSuccess = SetWaitableTimer(
		hEvents[eTimer],// Handle to the timer object.
		&liDueTime,		// When timer will become signaled.
		this->period,	// Periodic timer interval
		NULL,			// Completion routine.
		NULL,			// Argument to the completion routine.
		0);				// Do not restore a suspended system.

	if (!bSuccess) {
		printf("PTimer::SetWaitableTimer Error - %d\n", GetLastError());
	}
}

void PTimer::stopAComponent() {
	bool bSuccess = SetWaitableTimer(
		hEvents[eTimer], // Handle to the timer object.
		&liDueTime,		// When timer will become signaled.
		0,		// Periodic timer interval
		NULL,	// Completion routine.
		NULL,	// Argument to the completion routine.
		0);		// Do not restore a suspended system.

	if (!bSuccess) {
		printf("PTimer::SetWaitableTimer Error - %d\n", GetLastError());
	}

	PTask::stopAComponent();
}

void PTimer::processATaskMsg() {
	DWORD dwResult;
	dwResult = WaitForMultipleObjects(
		eESynchronizationObjectsEnd,	// number of objects in array
		this->hEvents,	// array of objects
		FALSE,			// wait for any object
		INFINITE		// wait forever
	);

	switch (dwResult) {
	case WAIT_OBJECT_0 + eComponentMsgQueue:
		// hHandles[eComponentMsgQueue] was signaled
		this->collectMsgs();
		ResetEvent(this->hEvents[eComponentMsgQueue]);
		break;
	case WAIT_OBJECT_0 + eTaskMsgQueue:
		// hHandles[eTaskMsgQueue] was signaled
		this->distributeAMsg();
		ResetEvent(this->hEvents[eTaskMsgQueue]);
		break;
	case WAIT_OBJECT_0 + eTimer:
		// hHandles[eTimer] was signaled
		this->addATargetMsg(Timer::eTimesUp);
		ResetEvent(this->hEvents[eTaskMsgQueue]);
//		printf("%s\n", this->getComponentName().c_str());
		break;
	case WAIT_TIMEOUT:
		printf("Wait timed out.\n");
		break;
	default:
		// Return value is invalid.
		printf("Wait error: %d\n", GetLastError());
		ExitProcess(0);
	}
}

