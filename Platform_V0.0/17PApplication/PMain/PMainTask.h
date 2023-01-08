//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PMAINTASK
#define PMAINTASK  "PMainTask"
#include "../typedef.h"
#define PMAINTASK_ID GETCLASSID(_ePMainTask_)

#include "../../15PTechnical/PTask/PTask.h"
#include "../../17PApplication/PLifecycleManager/PLifecycleManager.h"
#include "../../14PAspect/PException/PExceptionManager.h"

class PMainTask: public PTask {
private:
	PLifecycleManager lifecycleManager;
	PExceptionManager exceptionManager;

public:
	PMainTask(String name);
	~PMainTask();
	void initialize();
	void finalize();

	// as a Main
	void startCore();
	void stopCore();

	void processAMsg(Message *pMessage);
};

#endif /* TASKTHREADOS_ */
