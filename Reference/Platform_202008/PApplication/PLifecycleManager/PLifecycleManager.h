#pragma once

#include "../TypeDef.h"
#define PLIFECYCLEMANAGER "PLifecycleManager"  // class string name
#define PLIFECYCLEMANAGER_ID GET_MODULE_ID(EPApplication::ePLifecycleManager)  // class int id

#include "../../../../Core_Lib/Source/09Application/LifecycleManager/LifecycleManager.h"
#include "../../../../POSAL_Lib/Source/14PAspect/PBasicIO/PDisplay.h"

class PLifecycleManager : public LifecycleManager {
public:
	enum class EReceivers {
		eBegin = PLIFECYCLEMANAGER_ID,
		eEnd
	};

	PLifecycleManager(ClassInfo classInfo = ClassInfo(PLIFECYCLEMANAGER_ID, PLIFECYCLEMANAGER)) :
		LifecycleManager(classInfo)
	{}
	virtual ~PLifecycleManager() {}

	virtual void initialize() {}
	virtual void finalize() {}

	virtual void startTasks();

protected:
	virtual void registerSystemSchedulers(Scheduler* pMain) {
		this->registerAScheduler((unsigned)LifecycleManager::EReceivers::eMain, pMain);
	}
	virtual void registerSystemComponents() {
		this->registerAComponent((unsigned)LifecycleManager::EReceivers::eLifecycleManager, this);
	}
	virtual void registerAspects() {
		this->registerAnAspect(
			(unsigned)LifecycleManager::EReceivers::eDisplay, 
			(unsigned)Component::EReceivers::eDisplay,
			new PDisplay()
		);
	}
	virtual void allocateSystemComponents() {
	}
	virtual void associateSystemSendersNReceivers() {
	}
	virtual void associateAspects() {
	}
	virtual void associateSystemSourcesNTargets() {
	}
};
