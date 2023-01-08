//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef EXCEPTIONMANAGER
#define EXCEPTIONMANAGER "ExceptionManager"
#include "../Typedef.h"
#define EXCEPTIONMANAGER_ID GETCLASSID(_eExceptionManager_)

#include "../../03Core/Component/Component.h"
#include "../../03Core/Component/Log.h"
#include "../../07Application/LifecycleManager/LifecycleManager.h"

class ExceptionManager: public Component {
private:
	int count;
public:
	ExceptionManager(const String &name): 
		Component(name, ClassInfo(EXCEPTIONMANAGER_ID, EXCEPTIONMANAGER)) {
		this->count = 0;
	}
	virtual ~ExceptionManager() {}
	virtual void initialize() = 0;
	virtual void finalize() = 0;
	virtual void write(Log *pLog) = 0;

	void porcessALog(Log *pLog) {
		//		logQueue.enQueue(pLog);
		this->write(pLog);
		////////////////////////////////////////////////
		// delete
		////////////////////////////////////////////////
		pLog->destroy(Log::pMemoryManager);
	}
	void processAWarning(Log *pWarning) {
		//		logQueue.enQueue(pWarning);
		this->write(pWarning);
		////////////////////////////////////////////////
		// delete
		////////////////////////////////////////////////
		pWarning->destroy(Log::pMemoryManager);
	}
	void processAnError(Log *pError) {
		//		logQueue.enQueue(pError);
		this->write(pError);
		////////////////////////////////////////////////
		// delete
		////////////////////////////////////////////////
		pError->destroy(Log::pMemoryManager);
	}


	void processAMsg(Message *pMsg) {
		switch (pMsg->getType()) {
		case Log::mLog:
			this->porcessALog(dynamic_cast<Log *> (pMsg->getPArg()));
			this->count++;
			if (this->count > 10) {
				this->addAReceiverMsg(LifecycleManager::mPauseSystem, this->getLifecycleManager()->getUId(), 0, NULL, LifecycleManager::mPauseSystemReply);
				this->count = 0;
			}
			break;
		case Log::mWarning:
			this->processAWarning(dynamic_cast<Log *> (pMsg->getPArg()));
			break;
		case Log::mError:
			this->processAnError(dynamic_cast<Log *> (pMsg->getPArg()));
			break;
		case LifecycleManager::mPauseSystemReply:
			printf("ExceptionManager::eCriticalReply\n");
		//	this->addAReceiverMsg(LifecycleManager::mRunSystem, this->getLifecycleManager()->getUId(), 0, NULL, LifecycleManager::mRunSystemReply);
		//	this->addAReceiverMsg(LifecycleManager::mShutdownSystem, this->getLifecycleManager()->getUId(), 0, NULL, LifecycleManager::mShutdownSystemReply);
			this->addAReceiverMsg(LifecycleManager::mResetSystem, this->getLifecycleManager()->getUId(), 0, NULL, LifecycleManager::mResetSystemReply);
			break;
		default:
			break;
		}
	}
};

#endif