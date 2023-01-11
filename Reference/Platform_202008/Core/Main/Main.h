#pragma once
#include "../TypeDef.h"
#define MAIN "Main"  // class string name
#define MAIN_ID GET_MODULE_ID(EApplication::eMain)  // class int id

#include "../../../../Core_Lib/Source/03Core/Scheduler/Scheduler.h"
#include "../../../../Core_Lib/Source/09Application/LifecycleManager/LifecycleManager.h"

class Main: public Scheduler
{
public: 
	// event type - provided interface
	enum class EEventType {
		eBegin = MAIN_ID,
		eInitializeAsAMain, eInitializeAsAMainReply,
		eFinalizeAsAMain, eFinalizeAsAMainReply,
		eEnd
	};

	// event map
	virtual void processAEvent(Event* pEvent) {
		switch (EEventType(pEvent->getType())) {
		case EEventType::eInitializeAsAMain:
			this->initializeAsAMain(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eFinalizeAsAMain:
			this->finalizeAsAMain(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		default:
			Scheduler::processAEvent(pEvent);
			break;

		}
	}
	///////////////////////////////////////////////////////////////////////////////////
	Main(EventQueue *pEventQueue, ClassInfo classInfo = ClassInfo(MAIN_ID, MAIN)) :
		Scheduler(pEventQueue, classInfo), pLifecycleManager(nullptr) {
	}
	virtual ~Main() {}

	// as a Main
	virtual void initializeAsAMain(LifecycleManager* pLifecycleManager) {
		// register LifecycleManager as a Component
		this->pLifecycleManager = pLifecycleManager;
		this->allocateAComponent(this->pLifecycleManager);
		// send a eInitializeAsALifecycleManager event to LifecycleManager
		this->SendAEvent(
			pLifecycleManager->getUId(),
			(unsigned)LifecycleManager::EEventType::eInitializeAsALifecycleManager,
			0,
			this  // associate this with LifecycleManager
		);
		// start event processing as a scheduler
		Scheduler::run();
	}
	virtual void finalizeAsAMain() {
		this->pLifecycleManager->~LifecycleManager();
		delete(this->pLifecycleManager);
	}

protected:
	// as a Component
	virtual void initialize() {}
	virtual void finalize() {}

private:
	// event mapping functions
	virtual void initializeAsAMain(Event* pEvent) {
		this->initializeAsAMain((LifecycleManager*)pEvent->getPArg());
	}
	virtual void finalizeAsAMain(Event* pEvent) {
		this->finalizeAsAMain();
	}

	// direct association
	LifecycleManager* pLifecycleManager;
};
