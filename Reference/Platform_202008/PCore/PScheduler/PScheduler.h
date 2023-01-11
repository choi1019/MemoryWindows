#pragma once
#include "../TypeDef.h"
#define PSCHEDULER "PScheduler"  // class string name
#define PSCHEDULER_ID GET_MODULE_ID(EPCore::ePScheduler)  // class int id

#include "../../../../Core_Lib/Source/03Core/Scheduler/Scheduler.h"
#include "../../../../POSAL_Lib/Source/13PCore/PEventQueue/PEventQueue.h"

class PScheduler : public Scheduler {

public:
	enum class EEventType {
		eBegin = PSCHEDULER_ID,
		eEnd
	};

	// constructors & destructos
	PScheduler(ClassInfo classInfo = ClassInfo(PSCHEDULER_ID, PSCHEDULER)) :
		Scheduler(new PEventQueue(), classInfo)
		// dwThreadId(0), hThread(NULL)
	{}
	virtual ~PScheduler() {}
	virtual void createATask();
	virtual void startATask();

protected:

	// event mapping functions
	virtual void initialize() {}
	virtual void finalize() {}


};