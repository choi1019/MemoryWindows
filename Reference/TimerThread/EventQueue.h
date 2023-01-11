#pragma once

#include <vector>
using namespace std;
#include <Windows.h>

#define MAXSIZE 20
class EventQueue {
private:
	int buffer[MAXSIZE] = { 0 };
	unsigned size, front, rear;

	CRITICAL_SECTION CriticalSection;
	HANDLE* pHEvent;
	HANDLE hEmptySemaphore;
	HANDLE hFullSemaphore;

public:
	EventQueue(HANDLE* pHEvent) : pHEvent(pHEvent), size(0), front(0), rear(0) {
		// Initialize the critical section one time only.
		if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400)) {
			printf("InitializeCriticalSectionAndSpinCount error: %d\n", GetLastError());
		}
		*this->pHEvent = CreateEvent(
			NULL,   // default security attributes
			FALSE,  // auto-reset event object
			FALSE,  // initial state is nonsignaled
			NULL  // unnamed object
		);
		if (*this->pHEvent == NULL)
		{
			printf("CreateEvent error: %d\n", GetLastError());
			ExitProcess(0);
		}

		// counting semaphore
		this->hEmptySemaphore = CreateSemaphore(
			NULL,     // default security attributes
			0,  // initial count
			MAXSIZE,  // maximum count
			NULL);    // unnamed semaphore

		if (this->hEmptySemaphore == NULL)
		{
			printf("CreateEmptySemaphore error: %d\n", GetLastError());
		}

		this->hFullSemaphore = CreateSemaphore(
			NULL,     // default security attributes
			MAXSIZE,  // initial count
			MAXSIZE,  // maximum count
			NULL);    // unnamed semaphore

		if (this->hFullSemaphore == NULL)
		{
			printf("CreateFullSemaphore error: %d\n", GetLastError());
		}
	}
	virtual ~EventQueue() {
		// Release resources used by the critical section object.
		DeleteCriticalSection(&CriticalSection);
		CloseHandle(*this->pHEvent);
	}

	void push(int value) {
		DWORD dwWaitResult = WaitForSingleObject(
			this->hFullSemaphore,   // handle to semaphore
			INFINITE);					// zero-second time-out interval

		// Request ownership of the critical section
		EnterCriticalSection(&CriticalSection);

		this->buffer[this->front] = value;
		this->front = this->front++ % MAXSIZE;
		this->size++;

		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);

		ReleaseSemaphore(this->hEmptySemaphore, 1, NULL);
		// set event
		SetEvent(*this->pHEvent);
		printf("Thread::push(): %d\n", value);
	}

	int pop() {
		DWORD dwWaitResult = WaitForSingleObject(
			this->hEmptySemaphore,   // handle to semaphore
			INFINITE);					// zero-second time-out interval
		// Request ownership of the critical section.

		EnterCriticalSection(&CriticalSection);

		int value = this->buffer[this->rear];
		this->rear = this->rear++ % MAXSIZE;
		this->size--;

		// Release ownership of the critical section.
		LeaveCriticalSection(&CriticalSection);
		// set event
		ReleaseSemaphore(this->hFullSemaphore, 1, NULL);
		printf("Thread::pop(): %d\n", value);
		return value;
	}
};
