//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#include "PMainTask.h"
#include "../../15PTechnical/PMemoryManager/PMemory.h"

PMainTask::PMainTask()
{
}

PMainTask::~PMainTask()
{
}

void PMainTask::initialize() {

}

void PMainTask::finalize() {

}

// initialize pTaskMsgQueue, pComponentMsgVector
void PMainTask::startCore() {
	StaticSegment::initialize();
	DynamicSegment::initialize();

	Message *pMessage = new(Message::pMemoryManager) Message();
	Argument *pArgument = new(Argument::pMemoryManager) Argument();
	pMessage->destroy(Message::pMemoryManager);
	pArgument->destroy(Argument::pMemoryManager);
	printf("%s\n", __FUNCTION__);
}

// finalize pTaskMsgQueue, pComponentMsgVector
void PMainTask::stopCore() {

}
