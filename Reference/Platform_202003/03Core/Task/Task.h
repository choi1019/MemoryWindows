//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef TASK
#define TASK "Task"
#include "../TypeDef.h"
#define TASK_ID GETCLASSID(_eTask_)

#include "../Component/Component.h"
#include "../../02Utility/Map/Map.h"
#include "TaskMsgQueue.h"

static unsigned gTaskId = 0;

class Task : public Component
{
public:
	enum EMessageType {
		mMessageTypeBegin = TASK_ID,

		mStartATask, mStartATaskReply,
		mStartComponents, mStartComponentsReply,
		mStopComponents, mStopComponentsReply,
		mStopATask, mStopATaskReply,

		mPauseATask, mPauseATaskReply,
		mMessageTypeEnd
	};

	enum EState {
		sStateBegin = TASK_ID,

		sRunning,
		sStopped,

		sStateEnd
	};

	typedef _Pair<int, Component*> ComponentPair;
	typedef _Map<int, Component*> ComponentMap;
	typedef _Pair<int, Task *> TaskPair;
	typedef _Map<int, Task *> TaskMap;

private:
	EState eState;

	TaskMsgQueue *pTaskMsgQueue;
	ComponentMsgVector *pComponentMsgVector;

	ComponentMap allocatedComponents;
	TaskMap destinationTasks;

public:
	/////////////////////////////////////////////////////////
	// constructors & destructors
	/////////////////////////////////////////////////////////
	Task(String componentName, const ClassInfo& classInfo,
		TaskMsgQueue *pTaskMsgQueue, ComponentMsgVector *pComponentMsgVector)
		: eState(sStopped), pTaskMsgQueue(pTaskMsgQueue), pComponentMsgVector(pComponentMsgVector),
		  Component(componentName, gTaskId++, classInfo) {
	}
	virtual ~Task() {
		// Compoistion
		this->pTaskMsgQueue->finalize();
		this->pComponentMsgVector->finalize();
	}

	/////////////////////////////////////////////////////////
	// initialize & finalize
	/////////////////////////////////////////////////////////
	virtual void initialize() {
		Component::initialize();

		this->destinationTasks.clear();
		this->allocatedComponents.clear();

		this->pTaskMsgQueue->initialize();
		this->pComponentMsgVector->initialize();

		this->eState = EState::sRunning;
		printf("%s::initialize())\n", this->getComponentName().c_str());
	}
	virtual void finalize() {
		Component::finalize();

		this->pTaskMsgQueue->finalize();
		this->pComponentMsgVector->finalize();

		printf("%s::finalize())\n", this->getComponentName().c_str());
	}

	//////////////////////////////////////////////////////////////////
	// getters
	//////////////////////////////////////////////////////////////////
//	ComponentMap *getPAllocatedComponents() { return &this->allocatedComponents; }
	TaskMap *getPDestinationTaskMsgQueues() { return &this->destinationTasks; }

	//////////////////////////////////////////////////////////////////
	// configuration
	//////////////////////////////////////////////////////////////////
	// component allocation
	inline virtual void allocateAComponent(Component *pComponent) {
		pComponent->setTaskId(this->getTaskId());
		pComponent->setGeneratedMsgVector(this->pComponentMsgVector);
		this->allocatedComponents.insert(Make_Pair(pComponent->getComponentId(), pComponent));
	}
	// association to another task
	inline void associateADestinationTask(Task *pTask) {
		this->destinationTasks.insert(Make_Pair(pTask->getTaskId(), pTask));
	}
	////////////////////////////////////////////////////////////////////////
	// getters & setters
	////////////////////////////////////////////////////////////////////////
	inline TaskMsgQueue *getTaskMsgQueue() { return this->pTaskMsgQueue; }

protected:
	////////////////////////////////////////////////////////////////////////
	// collect and distirbute messages
	////////////////////////////////////////////////////////////////////////
	void collectMsgs() {
		for (Message *pMsg : *this->pComponentMsgVector) {
			unsigned taskId = pMsg->getTargetTaskId();
			Task *pTargetTask = this->destinationTasks.find(taskId)->second;
			pTargetTask->pTaskMsgQueue->add(pMsg);
		}
		this->pComponentMsgVector->clear();
	}

	void distributeAMsg() {
		Message* pMsg = this->pTaskMsgQueue->get();
		Component *pComponent = allocatedComponents.find(pMsg->getTargetComponentId())->second;
		if (pComponent == NULL) {
			printf("bug");
		}
		pComponent->processAMsgEx(pMsg);
	}

	//////////////////////////////////////////////////////////////////
	// process messages as a Task
	//////////////////////////////////////////////////////////////////
	virtual void processATaskMsg() = 0;

public:
	virtual void processTaskMsgs() {
		this->eState = sRunning;
		while (this->eState == sRunning) {
			this->processATaskMsg();
		}
		this->processATaskMsg();
		printf("%s stopping-pTaskMsgQueue: %d\n", this->getComponentName().c_str(), this->pTaskMsgQueue->getLength());
		printf("%s stopping-pComponentMsgVector: %d\n", this->getComponentName().c_str(), this->pComponentMsgVector->getLength());
	}

	virtual void startATask() = 0;
	virtual void stopATask() {
		this->eState = EState::sStopped;
	}

protected:
	//////////////////////////////////////////////////////////////////
	// process messages as a Component
	//////////////////////////////////////////////////////////////////
	virtual void startComponents() {
		// this method is trigged by the eStartComponents message 
		for (ComponentPair componentPair : this->allocatedComponents) {
			componentPair.second->startAComponent();
		}
		printf("%s::startComponents()=============\n", this->getComponentName().c_str());
	}

	virtual void stopComponents() {
		for (ComponentPair componentPair : this->allocatedComponents) {
			componentPair.second->stopAComponent();
		}
		printf("%s::stopComponents()==============\n", this->getComponentName().c_str());
	}


	virtual void processAMsg(Message *pMsg) {
		switch (pMsg->getType()) {
		case Task::mStartATask:
			// it is arleady running
			this->addAReplyMsg(pMsg);
			break;
		case Task::mStartComponents:
			this->startComponents();
			this->addAReplyMsg(pMsg);
			break;
		case Task::mStopComponents:
			this->stopComponents();
			this->addAReplyMsg(pMsg);
			break;
		case Task::mStopATask:
			this->stopATask();
			this->addAReplyMsg(pMsg);
			break;
		default:
			Component::processAMsg(pMsg);
			break;
		}
	}

public:
	////////////////////////////////////////////////////////////////////////////////////////
	virtual void printDestinations() {
		for (TaskPair taskPair : this->destinationTasks) {
			Task *pTask = taskPair.second;
			printf("	%s (%d, %d)\n", pTask->getComponentName().c_str(), pTask->getTaskId(), pTask->getComponentId());
		}
	}

	virtual void printComponentMap() {
		for (ComponentPair componentPair : this->allocatedComponents) {
			Component *pComponent = componentPair.second;
			printf("	%s (%d, %d)\n", pComponent->getComponentName().c_str(), pComponent->getTaskId(), pComponent->getComponentId());
			pComponent->printAssociations();
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////
};

#endif