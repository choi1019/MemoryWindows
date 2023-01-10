//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef LIFECYCLEMANAGER
#define LIFECYCLEMANAGER "LifecycleManager"
#include "../Typedef.h"
#define LIFECYCLEMANAGER_ID GETCLASSID(_eLifecycleManager_)

#include "../../03Core/Component/Component.h"
#include "../../03Core/Task/Task.h"

class LifecycleManager: public Component {
public:
	typedef _Map<int, Component*> ComponentMap;
	typedef _Pair<int, Component*> ComponentPair;

	typedef _Map<int, Task*> TaskMap;
	typedef _Pair<int, Task*> TaskPair;

	////////////////////////////////////////////////////////////////////////////////////
	// Message Processing
	////////////////////////////////////////////////////////////////////////////////////	
	enum EMessageType {
		mMessateTypeBegin = LIFECYCLEMANAGER_ID,
		mInitializeSystem, mInitializeSystemReply,
		mStartSystem, mStartSystemReply,
		mRunSystem, mRunSystemReply,
		mPauseSystem, mPauseSystemReply,
		mStopSystem, mStopSystemReply,
		mResetSystem, mResetSystemReply,

		mMessageTypeEnd
	};
protected:
	virtual void processAMsg(Message *pMsg);

	////////////////////////////////////////////////////////////////////////////////////
	// System Configuration
	////////////////////////////////////////////////////////////////////////////////////
protected:
	Task *pMainTask;
	Component *pExceptionManager;

private:
	ComponentMap systemComponentMap;
	ComponentMap componentMap;
	TaskMap systemTaskMap;
	TaskMap taskMap;

	void registerSystemTasks();
	void registerSystemComponents();
	void allocateSystemComponents();
	void associateSystemSourcesNTargets();
	void associateSystemSendersNReceivers();

protected:
	Component *findAComponent(String name);
	Task *findATask(String name);
	Task *findATask(unsigned taskId);

	Task* registerAUserTask(Task *pTask);
	Component* registerAUserComponent(Component *pComponent);
	void registerASystemTask(Task *pTask);
	void registerASystemComponent(Component *pComponent);

	void allocateAComponent(Task *pTask, Component *pComponent);
	void associateASourcesNATarget(Component *pSourceComponent, Component *pTargetComponent);
	void associateASenderNAReceiver(Component *pSenderComponent, Component *pReceiverComponent);

	// User Tasks and Components Configuration
	virtual void registerUserTasks() = 0;
	virtual void registerUserComponents() = 0;
	virtual void allocateUserComponents() = 0;
	virtual void associateUserSourcesNTargets() = 0;
	virtual void associateUserSendersNReceivers() = 0;

private:
	void registerTasks();
	void registerComponents();
	void allocateComponents();
	void associateSourcesNTargets();
	void associateSendersNReceivers();


	////////////////////////////////////////////////////////////////////////////////////
	// State Transitions
	////////////////////////////////////////////////////////////////////////////////////
	enum EState {
		sStateBegin = LIFECYCLEMANAGER_ID,
		sCoreStarted,
		sSuspended,
		sReady,
		sRunning,
		sStateEnd
	};
	EState eState;

	///////////////////////////////////////////////////////////////////////
	// State Machines
	///////////////////////////////////////////////////////////////////////protected:
	class StateMachine {
	protected:
		LifecycleManager *pLifecycleManager;
		Message *pTransaction;
		int count;
	public:
		StateMachine(LifecycleManager *pLifecycleManager) :
			pLifecycleManager(pLifecycleManager), pTransaction(NULL), count(0) {}
		virtual void processAMsg(Message *pMessage) = 0;
	};
	class Suspended : public StateMachine {
	public:
		Suspended(LifecycleManager *pLifecycleManager) : StateMachine(pLifecycleManager) {}
		void processAMsg(Message *pMessag);
	} suspended;
	class Ready : public StateMachine {
	public:
		Ready(LifecycleManager *pLifecycleManager) : StateMachine(pLifecycleManager) {}
		void processAMsg(Message *pMessag);
	} ready;
	class Running : public StateMachine {
	public:
		Running(LifecycleManager *pLifecycleManager) : StateMachine(pLifecycleManager) {}
		void processAMsg(Message *pMessag);
	} running;


public:
	LifecycleManager(String componentName, Task *pMain, Component *pExceptionManager);
	virtual ~LifecycleManager();

	// As a Component & a Task
	virtual void initialize();
	virtual void finalize();

	// methods
	void initializeCore();
	void initializeSystem();
	void finalizeSystem();
	void finalizeCore();

	// debug
	void displaySystemComponentMap();
	void displayComponentMap();
};

#endif /* LIFECYCLEMANAGER_ */
