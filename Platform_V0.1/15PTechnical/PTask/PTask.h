//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PTASK
#define PTASK  "PTask"
#include "../typedef.h"
#define PTASK_ID GETCLASSID(_ePTask_)

#include "../../03Core/Task/Task.h"
#include "../../15PTechnical/PTask/PTaskMsgQueue.h"
#include "../../15PTechnical/PTask/PComponentMsgVector.h"

class PTask: public Task {
private:
	enum ESynchronizationObjects {
		eComponentMsgQueue,
		eTaskMsgQueue,
		eESynchronizationObjectsEnd
	};
	HANDLE hEvents[eESynchronizationObjectsEnd];


	HANDLE hThread;
	DWORD dwThreadID;

	PTaskMsgQueue taskMsgQueue;
	PComponentMsgVector componentMsgVector;

public:
	// for Task
	PTask(String name);
	// for MainTask
	PTask(String name, ClassInfo classInfo);
	// for TimerTask
	PTask(String name, ClassInfo classInfo, HANDLE *pHTaksMsgQueue, HANDLE *pHComponentMsgQueue);
	virtual ~PTask();

	virtual void initialize();
	virtual void finalize();
	virtual void processATaskMsg();

	virtual void startATask();
	virtual void stopATask();

	virtual void processAMsg(Message *pMessage);
};

#endif /* TASKTHREADOS_ */
