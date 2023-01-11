//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "PTaskMsgQueue.h"

PTaskMsgQueue::PTaskMsgQueue(HANDLE *pEventHandle) : pEventHandle(pEventHandle)
{
}

PTaskMsgQueue::~PTaskMsgQueue() {
	CloseHandle(*this->pEventHandle);
}

void PTaskMsgQueue::initialize() {
	TaskMsgQueue::initialize();

	InitializeSRWLock(&SRWLock);
	// counting semaphore
	*this->pEventHandle = CreateSemaphore(NULL, 0, MAXTASKMSGQUEUELENGTH, NULL);
	if (*this->pEventHandle == NULL)
	{
		printf("TaskQueueOS::preRun - CreateSemaphore failed: %d", GetLastError());
	}
	this->msgQueue.clear();
}

void PTaskMsgQueue::finalize() {
	TaskMsgQueue::finalize();

	while (this->msgQueue.getLength() > 0) {
		Message *pMessage = this->msgQueue.deQueue();
		printf(" PTaskMsgQueue::finalize() - %d\n", pMessage->getId());
//		if (pMessage->getPArg()) {
//			pMessage->getPArg()->~ValueObject();
//			Message::pMemoryManager->safeFree(pMessage);
//		}
//		pMessage->~Message();
//		Message::pMemoryManager->safeFree(pMessage);
	}
	CloseHandle(*this->pEventHandle);
}

int PTaskMsgQueue::add(Message *pMsg) {
	/////////////////////////////////////////////////////////////
	// critical section
	AcquireSRWLockExclusive(&SRWLock);
	this->msgQueue.enQueue(pMsg);
	ReleaseSRWLockExclusive(&SRWLock);
	/////////////////////////////////////////////////////////////
	// signaling
	ReleaseSemaphore(*pEventHandle, 1, NULL);
	return 0;
}

Message* PTaskMsgQueue::get() {
	/////////////////////////////////////////////////////////////
	// critical section
	AcquireSRWLockExclusive(&SRWLock);
	Message *pMsg = this->msgQueue.deQueue();
	ReleaseSRWLockExclusive(&SRWLock);
	/////////////////////////////////////////////////////////////
	return pMsg;
}

unsigned PTaskMsgQueue::getLength() {
	return this->msgQueue.getLength();
}