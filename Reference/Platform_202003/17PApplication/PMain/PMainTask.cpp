//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#include "PMainTask.h"

PMainTask::PMainTask(String name):
	lifecycleManager(this, &this->exceptionManager), 
	PTask(name, Make_ClassInfo(PMAINTASK_ID, PMAINTASK))
{
	this->setLifecycleManager(&this->lifecycleManager);
	this->setExceptionManager(&this->exceptionManager);
}

PMainTask::~PMainTask()
{
}

void PMainTask::initialize() {
	PTask::initialize();
}

void PMainTask::finalize() {
	PTask::finalize();
}

// initialize pTaskMsgQueue, pComponentMsgVector
void PMainTask::startCore() {
	Memory::initializeStaticSegment();
	Memory::initializeDynamicSegment();

	this->lifecycleManager.initializeCore();
	this->addAReceiverMsg(LifecycleManager::mInitializeSystem, this->lifecycleManager.getUId(),
		0, NULL, LifecycleManager::mInitializeSystemReply);
	this->processTaskMsgs();
}

// finalize pTaskMsgQueue, pComponentMsgVector
void PMainTask::stopCore() {
	Task::stopATask();
}

void PMainTask::processAMsg(Message *pMessage) {
	Log *pLog = NULL;
	switch (pMessage->getType()) {
	case LifecycleManager::mInitializeSystemReply:
		this->addAReceiverMsg(LifecycleManager::mStartSystem, this->lifecycleManager.getUId(), 
			0, NULL, LifecycleManager::mStartSystemReply);
		break;
	case LifecycleManager::mStartSystemReply:
		this->addAReceiverMsg(LifecycleManager::mRunSystem, this->lifecycleManager.getUId(),
			0, NULL, LifecycleManager::mRunSystemReply);
		break;
	case LifecycleManager::mRunSystemReply:
		printf("%s-mRunSystemReply()\n", __FUNCTION__);
		break;
	case LifecycleManager::mStopSystem:
		this->stopCore();
		this->addAReceiverMsg(LifecycleManager::mResetSystem, this->getUId(), 0, NULL, LifecycleManager::mResetSystemReply);
		printf("%s-mStopSystem()\n", __FUNCTION__);
		break;
	case LifecycleManager::mResetSystem:
		this->stopCore();
		this->addAReceiverMsg(LifecycleManager::mResetSystem, this->getUId(), 0, NULL, LifecycleManager::mResetSystemReply);
		printf("%s-mResetSystem()\n", __FUNCTION__);
		break;
	default:
		PTask::processAMsg(pMessage);
		break;
	}

}