//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "PComponentMsgVector.h"

PComponentMsgVector::PComponentMsgVector(HANDLE *pEventHandle) : pEventHandle(pEventHandle){
}

PComponentMsgVector::~PComponentMsgVector() {

}

void PComponentMsgVector::initialize() {
	ComponentMsgVector::initialize();
	*this->pEventHandle = CreateSemaphore(NULL, 0, MAXCOMPONENTMSGVECTORLENGTH, NULL);
	if (*this->pEventHandle == NULL)
	{
		printf("TaskQueueOS::preRun - CreateSemaphore failed : %d", GetLastError());
	}
	this->vector.clear();
}

void PComponentMsgVector::finalize() {
	ComponentMsgVector::finalize();

	for (Message *pMessage: this->vector) {
		printf(" PTaskMsgQueue::finalize() - %d\n", pMessage->getId());
//		pMessage->~Message();
//		Message::pMemoryManager->safeFree(pMessage);
	}
	CloseHandle(*this->pEventHandle);
}

_VectorIterator<Message *> PComponentMsgVector::begin() {
	return this->vector.begin();
}

_VectorIterator<Message *> PComponentMsgVector::end() {
	return this->vector.end();
}

void PComponentMsgVector::add(Message *pMsg) {
	this->vector.add(pMsg);
	// signaling
	ReleaseSemaphore(*pEventHandle, 1, NULL);
}

void PComponentMsgVector::clear() {
	this->vector.clear();
}

unsigned PComponentMsgVector::getLength() {
	return this->vector.getLength();
}