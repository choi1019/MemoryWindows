#pragma once
#include "../TypeDef.h"
#define SCHEDULER "Scheduler"  // class string name
#define SCHEDULER_ID GET_MODULE_ID(ECore::eScheduler)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/Component.h"
#include "../../../../Core_Lib/Source/03Core/EventQueue/EventQueue.h"

class Scheduler : public Component {
	/////////////////////////////////////////////////////////////////////////////
	// 1. start scheduler - event processing: run() {}
	// 2. allocate components: void allocateAComponent(Component* pComponent) {}
	/////////////////////////////////////////////////////////////////////////////
public:
	enum class EEventType {
		eBegin = SCHEDULER_ID,
		eInitializeAsAScheduler, eInitializeAsASchedulerReply,
		eAllocateAComponent, eAllocateAComponentReply,
		eDellocateAComponent, eDellocateAComponentReply,
		eFinalizeAsAScheduler, eFinalizeAsASchedulerReply,
		eEnd
	};
	enum class EState {
		eIdle,
		eRunning,
		eStateEnd
	};

	// constructors & destructos
	Scheduler(EventQueue *pEventQueue, ClassInfo classInfo = ClassInfo(SCHEDULER_ID, SCHEDULER)) :
		Component(classInfo),
		eState(EState::eIdle),
		pEventQueue(pEventQueue)
	{
		this->pEventQueue->setSchedulerId(this->getComponentId());
		this->allocateAComponent(this);
	}
	virtual ~Scheduler() {
		delete(this->pEventQueue);
	}

	virtual void createATask() {}
	virtual void startATask() {}

	// callback
	virtual void run() {
		this->eState = EState::eRunning;

		while (this->eState == EState::eRunning) {
			this->runOnce();
		}
	};

	virtual void processAEvent(Event* pEvent) {
		switch (EEventType(pEvent->getType())) {
		case EEventType::eInitializeAsAScheduler:
			this->initializeAsAScheduler(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eFinalizeAsAScheduler:
			this->finalizeAsAScheduler(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eAllocateAComponent:
			this->allocateAComponent(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eDellocateAComponent:
			this->delocateAComponent(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		default:
			Component::processAEvent(pEvent);
			break;

		}
	}

protected:
	virtual void initialize() {}
	virtual void finalize() {}

	// main call this directly
	void allocateAComponent(Component* pAllocatedComponent) {
		// associate this eventQueue to a component
		pAllocatedComponent->allocate(this->pEventQueue);
		this->allocatedComponentMap[pAllocatedComponent->getComponentId()] = pAllocatedComponent;
	}
	void delocateAComponent(Component* pDelocatedComponent) {
//		this->allocatedComponentMap.erase(pDelocatedComponent->getComponentId());
	}

	virtual void runOnce() {
		Event* pEvent = this->pEventQueue->pop();
		Component* pTargetComponent = this->allocatedComponentMap[pEvent->getTargetUId().getComponentId()];
		pTargetComponent->processAEvent(pEvent);

		pEvent->destroy(Event::spMemoryManager);
	}

private:
	// attributes
	EState eState;

	// associations
	EventQueue* pEventQueue;
	// componentId, component *
	map<unsigned, Component*, MAXCOMPONENTSALLOCATED> allocatedComponentMap;

	////////////////////////////////////////////////////////////////
	// event mapping functions
	////////////////////////////////////////////////////////////////
	virtual void initializeAsAScheduler(Event* pEvent) {
	}
	virtual void finalizeAsAScheduler(Event* pEvent) {
		this->allocatedComponentMap.clear();
	}
	void allocateAComponent(Event* pEvent) {
		this->allocateAComponent((Component*)pEvent->getPArg());
	}
	void delocateAComponent(Event* pEvent) {
		this->delocateAComponent((Component*)pEvent->getPArg());
	}


};
