//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PTIMER
#define PTIMER  "PTimer"
#include "../typedef.h"
#define PTIMER_ID GETCLASSID(_ePTimer_)

#include "../../15PTechnical/PTask/PTask.h"
#include "../../05Technical/Timer/Timer.h"

class PTimer : public PTask, public Timer {

private:
	enum ESynchronizationObjects {
		eTaskMsgQueue, 
		eComponentMsgQueue,
		eTimer,
		eESynchronizationObjectsEnd
	};
	HANDLE hEvents[eESynchronizationObjectsEnd];

	__int64 initialDelay;
	DWORD period;

	LARGE_INTEGER   liDueTime;	// period
	
public:
	// constructos
	PTimer(String componentName, __int64 initialDelay, DWORD period);
	~PTimer();

	void initialize();
	void finalize();

	void startAComponent();
	void stopAComponent();

	void processATaskMsg();
};

#endif /* TIMER_ */
