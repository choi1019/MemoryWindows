//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PLIFECYCLEMANAGER
#define PLIFECYCLEMANAGER "PLifecycleManager"
#include "../Typedef.h"
#define PLIFECYCLEMANAGER_ID GETCLASSID(_ePLifecycleManager_)

#include "../../07Application/LifecycleManager/LifecycleManager.h"
#include "../../04Aspect/Exception/ExceptionManager.h"
#include "../../15PTechnical/PTask/PTask.h"
#include "../../15PTechnical/PTimer/PTimer.h"
#include "../../06Domain/Sender/Sender.h"
#include "../../06Domain/Receiver/Receiver.h"

class PLifecycleManager : public LifecycleManager {
private:
	Task *pTask1, *pTask2, *pTask3;
	Task *pTimer1, *pTimer2;
	Component *pSender, *pReceiver;

public:
	PLifecycleManager(PTask *pMainTask, ExceptionManager *pExceptionManager);
	virtual ~PLifecycleManager();

protected:
	void registerUserTasks();
	void registerUserComponents();
	void allocateUserComponents();
	void associateUserSourcesNTargets();
	void associateUserSendersNReceivers();
};

#endif /* LIFECYCLEMANAGERPRODUCT_ */