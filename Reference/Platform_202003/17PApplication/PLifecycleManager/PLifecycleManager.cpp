//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#include "PLifecycleManager.h"

PLifecycleManager::PLifecycleManager(PTask *pMainTask, ExceptionManager *pExceptionManager)
	: LifecycleManager(PLIFECYCLEMANAGER, pMainTask, pExceptionManager)
{
}

PLifecycleManager::~PLifecycleManager() {
}

void PLifecycleManager::registerUserTasks() {
	this->pTimer1 = this->registerAUserTask(new PTimer("Timer1", 100, 2));
	this->pTimer2 = this->registerAUserTask(new PTimer("Timer2", 100, 5));
}

void PLifecycleManager::registerUserComponents() {
	this->pSender = this->registerAUserComponent(new Sender("Sender"));
	this->pReceiver = this->registerAUserComponent(new Receiver("Receiver"));
}

///////////////////////////////////////////
void PLifecycleManager::allocateUserComponents() {
//	this->allocateAComponent(this->pMainTask, this->pTimer10);
//	this->allocateAComponent(this->pTask1, this->pTimer100);
	this->allocateAComponent(this->pTimer1, this->pSender);
	this->allocateAComponent(this->pTimer2, this->pReceiver);
}

void PLifecycleManager::associateUserSourcesNTargets() {
	this->associateASourcesNATarget(this->pTimer1, pSender);
	this->associateASourcesNATarget(this->pTimer2, pSender);
}

void PLifecycleManager::associateUserSendersNReceivers() {
	this->associateASenderNAReceiver(this->pSender, this->pReceiver);
}
///////////////////////////////////////////