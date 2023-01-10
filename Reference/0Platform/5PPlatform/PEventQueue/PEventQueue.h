#pragma once

#include "../typedef.h"
#define _PEventQueue_Id _GET_CLASS_UID(_EPPlatform::_ePEventQueue)
#define _PEventQueue_Name "PEventQueue"


#include "../../2Platform/EventQueue/EventQueue.h"
#include <Windows.h>

#define MAXLENGTH_EVENTQUEUE 20

class PEventQueue: public EventQueue {
private:
	CRITICAL_SECTION m_CriticalSection;

	HANDLE ghSemaphoreEmpty;
	HANDLE ghSemaphoreFull;

public:
	PEventQueue(
		int nSchedulerId,
		int nClassId = _PEventQueue_Id, 
		const char* pcClassName = _PEventQueue_Name)
		: EventQueue(nSchedulerId, nClassId, pcClassName)
	{
		// Create a semaphore with initial and max counts of MAX_SEM_COUNT
		ghSemaphoreEmpty = CreateSemaphore(
			NULL,					// default security attributes
			0,						// initial count
			MAXLENGTH_EVENTQUEUE,	// maximum count
			NULL);					// unnamed semaphore

		if (ghSemaphoreEmpty == NULL)
		{
			printf("CreateSemaphore error: %d\n", GetLastError());
		}

		ghSemaphoreFull = CreateSemaphore(
			NULL,					// default security attributes
			MAXLENGTH_EVENTQUEUE,						// initial count
			MAXLENGTH_EVENTQUEUE,	// maximum count
			NULL);					// unnamed semaphore

		if (ghSemaphoreFull == NULL)
		{
			printf("CreateSemaphore error: %d\n", GetLastError());
		}

		InitializeCriticalSection(&m_CriticalSection);

	}

	virtual ~PEventQueue() 
	{
		DeleteCriticalSection(&m_CriticalSection);

		CloseHandle(ghSemaphoreEmpty);
		CloseHandle(ghSemaphoreFull);
	}

	virtual void PushFront(Event* pEvent) {
		// if queue full, wait
		DWORD dwWaitResult = WaitForSingleObject(ghSemaphoreFull, INFINITE);
		EnterCriticalSection(&m_CriticalSection);
		EventQueue::PushFront(pEvent);
		LeaveCriticalSection(&m_CriticalSection);

		if (!ReleaseSemaphore(ghSemaphoreEmpty, 1, NULL))
		{
			printf("Release ghSemaphoreEmpty error: %d\n", GetLastError());
		}

	}

	virtual void PushBack(Event* pEvent) {
		// if queue full, wait
		DWORD dwWaitResult = WaitForSingleObject(ghSemaphoreFull, INFINITE);
		EnterCriticalSection(&m_CriticalSection);
		EventQueue::PushBack(pEvent);
		LeaveCriticalSection(&m_CriticalSection);

		if (!ReleaseSemaphore(ghSemaphoreEmpty, 1, NULL))
		{
			printf("Release ghSemaphoreEmpty error: %d\n", GetLastError());
		}
	}

	virtual Event* Pop() {
		// if queue empty, wait
		DWORD dwWaitResult = WaitForSingleObject(ghSemaphoreEmpty, INFINITE);
		EnterCriticalSection(&m_CriticalSection);
		Event* pEvent = EventQueue::Front();

		if (pEvent->GetCoundChildren() > 0) {
			// wait unitil all the child events are replied
			pEvent = nullptr;
			if (!ReleaseSemaphore(ghSemaphoreEmpty, 1, NULL))
			{
				printf("Release ghSemaphoreEmpty error: %d\n", GetLastError());
			}
		} else {
			EventQueue::Pop();
			if (!ReleaseSemaphore(ghSemaphoreFull, 1, NULL))
			{
				printf("Release ghSemaphoreFull error: %d\n", GetLastError());
			}
		}
		LeaveCriticalSection(&m_CriticalSection);
		return pEvent;
	}
};
