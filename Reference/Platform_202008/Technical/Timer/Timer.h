#pragma once
#include "../TypeDef.h"
#define TIMER "Timer"  // class string name
#define TIMER_ID GET_MODULE_ID(ETechnical::eTimer)  // class int id

#include "../../../../Core_Lib/Source/00Global/Object/Object.h"

class Timer {

public:
	enum class EEventType {
		eBegin = TIMER_ID,
		eTimesUp, eTimesUpReply,
		eEnd
	};
	static Timer* vectorPPTimer[];

	// constructors & destructos
	Timer(unsigned dueTime, unsigned interval, ClassInfo classInfo = ClassInfo(TIMER_ID, TIMER));
	virtual ~Timer();

	virtual void startTimer();
	virtual void timesUp();

protected:
	unsigned dueTime, interval;
};