
#ifndef MAIN
#define MAIN "MAIN"
#include "../TypeDef.h"
#define MAIN_ID GET_COMPONENT_ID(eMain_SubLayer)

#include "../../03Core/Task/Task.h"
#include "../../05Technical/Timer/Timer.h"
#include "../../07Application/LifecycleManager/LifecycleManager.h"

class MainTask: public Task
{
protected:
	LifecycleManager *pLifecycleManager;
public:
	MainTask(String name, ClassInfo classInfo, LifecycleManager *pLifecycleManager):
		pLifecycleManager(pLifecycleManager), Task(name, classInfo) {
	}
	virtual ~MainTask() {
	}
	virtual void initialize() {
		Task::initialize();
	}
	virtual void finalize() {
		Task::finalize();
	}

	void initializeSystem() {
		pLifecycleManager->initializeSystem();
	}
	void startSystem() {
		pLifecycleManager->startSystem();
	}
	void finalizeSystem() {
		pLifecycleManager->finalizeSystem();
	}
};

#endif