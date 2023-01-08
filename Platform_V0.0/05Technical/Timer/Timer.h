//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef TIMER
#define TIMER "Timer"
#include "../TypeDef.h"
#define TIMER_ID GETCLASSID(_eTimer_)

class Timer {
public:
	enum EMessageType {
		eMessageTypeBegin = TIMER_ID,
		eTimesUp, eTimesUpReply,
		eChangeTimer, eTimerChanged,
		eMessageTypeEnd
	};

public:
	// constructos
	Timer() {}
	~Timer() {}
};

#endif /* TIMER */

