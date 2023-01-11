#pragma once
#include "../TypeDef.h"
#define PTIMER "PTimer"  // class string name
#define PTIMER_ID GET_MODULE_ID(EPTechnical::ePTimer)  // class int id

#include "../../../../Technical_Lib/Source/05Technical/Timer/Timer.h"
#include "../../../../POSAL_Lib/Source/13PCore/PScheduler/PScheduler.h"

/* FreeRTOS.org includes. */
#include "../../../../FreeRTOS_Lib/Source/include/FreeRTOS.h"
#include "../../../../FreeRTOS_Lib/Source/include/timers.h"


#define MAX_TIMER_COUNT 10

class PTimer : public PScheduler, public Timer {

public:
	enum class ETargetGroups {
		eBegin = PTIMER_ID,
		e100u,
		e1m,
		eEnd
	};


	// constructors & destructos
	PTimer(unsigned dueTime = 1, unsigned interval = 100, ClassInfo classInfo = ClassInfo(PTIMER_ID, PTIMER));
	virtual ~PTimer();
	void initialize();
	void finalize();

	virtual void timesUp() {
//		cout << _methodName() << "\n";
		this->SendTargetEvents((unsigned)ETargetGroups::e100u, (unsigned)Timer::EEventType::eTimesUp);
	}
private:
	static uint32_t sId;
	TimerHandle_t xAutoReloadTimer;
	BaseType_t xTimerStarted;
};
