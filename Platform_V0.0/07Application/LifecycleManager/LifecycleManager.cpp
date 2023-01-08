//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#include "LifecycleManager.h"

 //////////////////////////////////////////////////////////////////////////////////
 // constructors
 //////////////////////////////////////////////////////////////////////////////////

LifecycleManager::LifecycleManager(String componentName, Task* pMainTask, Component *pExceptionManager) :
	eState(EState::sSuspended),
	pMainTask(pMainTask), pExceptionManager(pExceptionManager),
	suspended(this), ready(this), running(this),
	Component(componentName, Make_ClassInfo(LIFECYCLEMANAGER_ID, LIFECYCLEMANAGER))
{
}
LifecycleManager::~LifecycleManager() {
}

void LifecycleManager::initialize() {
	Component::initialize();
}

void LifecycleManager::finalize() {
	Component::finalize();
}

//////////////////////////////////////////////////////////////////////////////////
// utility
//////////////////////////////////////////////////////////////////////////////////

Component* LifecycleManager::findAComponent(String name) {
	// for each detination TaskCoreId, find the real TaskCore
	for (ComponentPair itrComponent : this->componentMap) {
		if (itrComponent.second->getComponentName() == name) {
			return itrComponent.second;
		}
	}
	return NULL;
}

// find a TaskCore by Id
Task *LifecycleManager::findATask(String name) {
	// for each detination TaskCoreId, find the real TaskCore
	for (TaskPair itrTaskCore : this->taskMap) {
		if (itrTaskCore.second->getComponentName() == name) {
			return itrTaskCore.second;
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////
// Task Registartion
//////////////////////////////////////////////////////////////////////////////////

void LifecycleManager::registerASystemTask(Task *pTask) {
	this->systemTaskMap.insert(Make_Pair(pTask->getTaskId(), pTask));
	// A task is a Component
	this->registerASystemComponent(pTask);
}
void LifecycleManager::registerSystemTasks() {
	// register PMain
	this->registerASystemTask(this->pMainTask);
}

Task *LifecycleManager::registerAUserTask(Task *pTask) {
	this->taskMap.insert(Make_Pair(pTask->getTaskId(), pTask));
	// A task is a Component
	this->registerAUserComponent(pTask);
	return pTask;
}
void LifecycleManager::registerTasks() {
	this->registerUserTasks();
}

//////////////////////////////////////////////////////////////////////////////////
// Component Registartion
//////////////////////////////////////////////////////////////////////////////////

void LifecycleManager::registerASystemComponent(Component *pComponent) {
	pComponent->setLifecycleManager(this);
	pComponent->setExceptionManager(this->pExceptionManager);
	this->systemComponentMap.insert(Make_Pair(pComponent->getComponentId(), pComponent));
}
void LifecycleManager::registerSystemComponents() {
	this->registerASystemComponent(this);
	this->registerASystemComponent(this->pExceptionManager);
}

Component *LifecycleManager::registerAUserComponent(Component *pComponent) {
	pComponent->setLifecycleManager(this);
	pComponent->setExceptionManager(this->pExceptionManager);
	this->componentMap.insert(Make_Pair(pComponent->getComponentId(), pComponent));
	return pComponent;
}
void LifecycleManager::registerComponents() {
	this->registerUserComponents();
}

//////////////////////////////////////////////////////////////////////////////////
// Component allocation
//////////////////////////////////////////////////////////////////////////////////
void LifecycleManager::allocateAComponent(Task *pTaskCore, Component *pComponent) {
	pTaskCore->allocateAComponent(pComponent);
}

void LifecycleManager::allocateSystemComponents() {
	// allocate a lifecycleManager as a Component of MainTask
	this->allocateAComponent(this->pMainTask, this);
	// allocate a exceptionManager as a Component of MainTask
	this->allocateAComponent(this->pMainTask, this->pExceptionManager);

	// allocate a systemTask as a component of itself
	for (TaskPair taskPair : this->systemTaskMap) {
		taskPair.second->allocateAComponent(taskPair.second);
	}
}

void LifecycleManager::allocateComponents() {
	// allocate a task as a component of itself
	for (TaskPair taskPair : this->taskMap) {
		taskPair.second->allocateAComponent(taskPair.second);
	}
	this->allocateUserComponents();
}

//////////////////////////////////////////////////////////////////////////////////
// Source N Target Associations
//////////////////////////////////////////////////////////////////////////////////
Task *LifecycleManager::findATask(unsigned taskId) {
	TaskMap::Iterator foundTaskIterator = this->taskMap.find(taskId);
	if (foundTaskIterator == this->taskMap.end()) {
		foundTaskIterator = this->systemTaskMap.find(taskId);
		if (foundTaskIterator == this->taskMap.end()) {
			/////////////////////////////////////////////////// exception
			return NULL;
		}
	}
	return foundTaskIterator->second;
}

void LifecycleManager::associateASourcesNATarget(Component *pSourceComponent, Component *pTargetComponent) {
	// associate a target Component to a source Component
	pSourceComponent->associateATarget(pTargetComponent);

	// associate a target TaskCore to asource TaskCore
	Task *pSourceTask = this->findATask(pSourceComponent->getTaskId());
	Task *pTargetTask = this->findATask(pTargetComponent->getTaskId());
	pSourceTask->associateADestinationTask(pTargetTask);
	pTargetTask->associateADestinationTask(pSourceTask);
}

void LifecycleManager::associateSystemSourcesNTargets() {
}

void LifecycleManager::associateSourcesNTargets() {
	this->associateUserSourcesNTargets();
}

//////////////////////////////////////////////////////////////////////////////////
// Sender N Receiver Associations
//////////////////////////////////////////////////////////////////////////////////
void LifecycleManager::associateASenderNAReceiver(Component *pSenderComponent, Component *pReceiverComponent) {
	pSenderComponent->associateAReceiver(pReceiverComponent);

	// associate a target TaskCore to a source TaskCore
	Task *pSenderTask = this->findATask(pSenderComponent->getTaskId());
	Task *pReceiverTask = this->findATask(pReceiverComponent->getTaskId());
	pSenderTask->associateADestinationTask(pReceiverTask);
	pReceiverTask->associateADestinationTask(pSenderTask);
}

void LifecycleManager::associateSystemSendersNReceivers() {
	for (ComponentPair itrComponent : this->systemComponentMap) {
		// associate self as a target for recursive message
		this->associateASenderNAReceiver(itrComponent.second, itrComponent.second);

		// associate LifecycleMnager to all components
		this->associateASenderNAReceiver(this, itrComponent.second);
		// associate all components to LifecycleManager
		this->associateASenderNAReceiver(itrComponent.second, this);
		// associate all components to ExceptionManager
		this->associateASenderNAReceiver(itrComponent.second, this->pExceptionManager);
	}
}

void LifecycleManager::associateSendersNReceivers() {
	for (ComponentPair itrComponent : this->componentMap) {
		// associate self as a target for recursive message
		this->associateASenderNAReceiver(itrComponent.second, itrComponent.second);

		// associate LifecycleMnager to all components
		this->associateASenderNAReceiver(this, itrComponent.second);
		// associate all components to LifecycleManager
		this->associateASenderNAReceiver(itrComponent.second, this);
		// associate all components to ExceptionManager
		this->associateASenderNAReceiver(itrComponent.second, this->pExceptionManager);
	}
	this->associateUserSendersNReceivers();
}

//////////////////////////////////////////////////////////////////////////////////
// initlaize 
//////////////////////////////////////////////////////////////////////////////////
// System
void LifecycleManager::initializeCore() {
	this->systemTaskMap.clear();
	this->systemComponentMap.clear();

	///////////////////////////////////////////////////////
	// register Tasks & Components
	///////////////////////////////////////////////////////
	this->registerSystemTasks();
	this->registerSystemComponents();

	// initialize Components
	for (ComponentPair componentPair : this->systemComponentMap) {
		componentPair.second->initialize();
	}

	///////////////////////////////////////////////////////
	// allocate
	///////////////////////////////////////////////////////
	this->allocateSystemComponents();

	///////////////////////////////////////////////////////
	// associate Components
	///////////////////////////////////////////////////////
	this->associateSystemSourcesNTargets();
	this->associateSystemSendersNReceivers();

	this->displaySystemComponentMap();
}

void LifecycleManager::finalizeCore() {
	for (TaskPair taskPair : this->systemTaskMap) {
		taskPair.second->finalize();
	}
}

void LifecycleManager::initializeSystem() {
	this->taskMap.clear();
	this->componentMap.clear();

	///////////////////////////////////////////////////////
	// register Tasks & Components
	///////////////////////////////////////////////////////
	this->registerTasks();
	this->registerComponents();

	// initialize Components
	for (ComponentPair componentPair : this->componentMap) {
		componentPair.second->initialize();
	}

	///////////////////////////////////////////////////////
	// allocate Components
	///////////////////////////////////////////////////////
	this->allocateComponents();

	///////////////////////////////////////////////////////
	// associate Components
	///////////////////////////////////////////////////////
	this->associateSourcesNTargets();
	this->associateSendersNReceivers();

	this->displayComponentMap();
}

void LifecycleManager::finalizeSystem() {
	for (TaskPair taskPair : this->taskMap) {
		taskPair.second->finalize();
	}
}

//////////////////////////////////////////////////////////////////////////////////
// State Transitions
//////////////////////////////////////////////////////////////////////////////////
void LifecycleManager::Suspended::processAMsg(Message *pMessage) {
	switch (pMessage->getType()) {
	case  EMessageType::mInitializeSystem:
		this->pLifecycleManager->initializeSystem();
		this->pLifecycleManager->addAReplyMsg(pMessage);
		break;
	case  EMessageType::mStartSystem:
		/////////////////////////////////////////////////////////////////////
		this->pTransaction = new(Message::pMemoryManager) Message(pMessage);
		/////////////////////////////////////////////////////////////////////
		for (TaskPair itrTask : this->pLifecycleManager->taskMap) {
			this->pLifecycleManager->addAReceiverMsg(
				Task::EMessageType::mStartATask, itrTask.second->getUId(), 0, NULL, Task::mStartATaskReply);
			itrTask.second->startATask();
		}
		this->count = this->pLifecycleManager->taskMap.getLength();
		break;
	case  Task::EMessageType::mStartATaskReply:
		this->count--;
		if (this->count == 0) {
			/////////////////////////////////////////////////////////////////////
			this->pLifecycleManager->addAReplyMsg(this->pTransaction);
			this->pTransaction->~Message();
			Message::pMemoryManager->safeFree(this->pTransaction);
			/////////////////////////////////////////////////////////////////////
			this->pLifecycleManager->eState = EState::sReady;
		}
		break;
	default:
		break;
	}
}

void LifecycleManager::Ready::processAMsg(Message *pMessage) {
	switch (pMessage->getType()) {
	case EMessageType::mRunSystem:
		/////////////////////////////////////////////////////////////////////
		this->pTransaction = new(Message::pMemoryManager) Message(pMessage);
		/////////////////////////////////////////////////////////////////////
		for (TaskPair itrTask : this->pLifecycleManager->taskMap) {
			this->pLifecycleManager->addAReceiverMsg(
				Task::EMessageType::mStartComponents, itrTask.second->getUId(), 0, NULL, Task::mStartComponentsReply);
		}
		this->count = this->pLifecycleManager->taskMap.getLength();
		break;
	case  Task::mStartComponentsReply:
		this->count--;
		if (this->count == 0) {
			this->pLifecycleManager->eState = EState::sRunning;
			this->pLifecycleManager->addAReplyMsg(this->pTransaction);
			/////////////////////////////////////////////////////////////////////
			this->pTransaction->~Message();
			Message::pMemoryManager->safeFree(this->pTransaction);
			/////////////////////////////////////////////////////////////////////
		}
		break;
	case EMessageType::mResetSystem:
	case EMessageType::mStopSystem:
		/////////////////////////////////////////////////////////////////////
		this->pTransaction = new(Message::pMemoryManager) Message(pMessage);
		/////////////////////////////////////////////////////////////////////
		for (TaskPair itrTask : this->pLifecycleManager->taskMap) {
			this->pLifecycleManager->addAReceiverMsg(
				Task::EMessageType::mStopATask, itrTask.second->getUId(), 0, NULL, Task::mStopATaskReply);
		}
		this->count = this->pLifecycleManager->taskMap.getLength();
		break;
	case Task::EMessageType::mStopATaskReply:
		this->count--;
		if (this->count == 0) {
			this->pLifecycleManager->finalizeSystem();
			this->pLifecycleManager->eState = EState::sSuspended;
			/////////////////////////////////////////////////////////////////////
			// return to mainTask - mStopSystem or mResetSystem
			this->pLifecycleManager->addAReceiverMsg(
				this->pTransaction->getType(), this->pLifecycleManager->pMainTask->getUId(), 0, NULL);

			/////////////////////////////////////////////////////////////////////
			this->pTransaction->~Message();
			Message::pMemoryManager->safeFree(this->pTransaction);
			/////////////////////////////////////////////////////////////////////
		}
		break;
	default:
		break;
	}
}

void LifecycleManager::Running::processAMsg(Message *pMessage) {
	switch (pMessage->getType()) {
	case EMessageType::mPauseSystem:
		/////////////////////////////////////////////////////////////////////
		this->pTransaction = new(Message::pMemoryManager) Message(pMessage);
		/////////////////////////////////////////////////////////////////////
		for (TaskPair itrTask : this->pLifecycleManager->taskMap) {
			this->pLifecycleManager->addAReceiverMsg(
				Task::EMessageType::mStopComponents, itrTask.second->getUId(), 0, NULL, Task::mStopComponentsReply);
		}
		this->count = this->pLifecycleManager->taskMap.getLength();
		break;
	case Task::EMessageType::mStopComponentsReply:
		this->count--;
		if (this->count == 0) {
			this->pLifecycleManager->eState = EState::sReady;
			/////////////////////////////////////////////////////////////////////
			this->pLifecycleManager->addAReplyMsg(this->pTransaction);
			this->pTransaction->~Message();
			Message::pMemoryManager->safeFree(this->pTransaction);
			/////////////////////////////////////////////////////////////////////
		}
		break;
	default:
		break;
	}
}

void LifecycleManager::processAMsg(Message *pMessage) {
	switch (this->eState) {
	case EState::sSuspended:
		this->suspended.processAMsg(pMessage);
		break;
	case  EState::sReady:
		this->ready.processAMsg(pMessage);
		break;
	case  EState::sRunning:
		this->running.processAMsg(pMessage);
		break;
	default:
		Component::processAMsg(pMessage);
		break;
	}	
}

void LifecycleManager::displaySystemComponentMap() {

	for (TaskPair itrTask : this->systemTaskMap) {
		Task *pTask = itrTask.second;
		printf(" %s: %d ===================================================\n", __FUNCTION__, __LINE__);
		printf("%s (%d, %d)\n", pTask->getComponentName().c_str(), pTask->getTaskId(), pTask->getComponentId());
		printf("  Destination Task Map\n");
		pTask->printDestinations();
		printf("  Component Map\n");
		pTask->printComponentMap();
	}
	printf("  ===================================================\n");
}

void LifecycleManager::displayComponentMap() {

	for (TaskPair itrTask : this->taskMap) {
		Task *pTask = itrTask.second;
		printf(" %s: %d ===================================================\n", __FUNCTION__, __LINE__);
		printf("%s (%d, %d)\n", pTask->getComponentName().c_str(), pTask->getTaskId(), pTask->getComponentId());
		printf("  Destination Task Map\n");
		pTask->printDestinations();
		printf("  Component Map\n");
		pTask->printComponentMap();
	}
	printf("  ===================================================\n");
}
