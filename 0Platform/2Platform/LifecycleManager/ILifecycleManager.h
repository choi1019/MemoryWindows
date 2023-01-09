#pragma once

#include "../typedef.h"
#define _LifecycleManager_Id _GET_CLASS_UID(_EPlatform::_eLifecycleManager)
#define _LifecycleManager_Name "LifecycleManager"

#include "../../1Base/Object/BaseObject.h"
#include "../../2Platform/Scheduler/Scheduler.h"

class ILifecycleManager {
public:
	enum class EEventType {
		eBegin = _LifecycleManager_Id,

//		eStartSystem,
		eStopSystem,

		eInitializeAsALifecycleManager,
		eFinalizeAsALifecycleManager,

		eRegisterSchedulers,
		eInitializeSchedulers,
		eStartSchedulers,
		eFinalizeSchedulers,
		eStopSchedulers,

		eRegisterComponents,
		eAllocateComponents,

		eAssociateSendersNReceivers,
		eAssociateSourcesNTargets,

		eInitializeComponents,
		eFinalizeComponents,

		eStartComponents,
//		eStopComponents,
		eEnd
	};


	enum class EReceivers {
		eBegin = _LifecycleManager_Id,
		eMainScheduler,
		eLifecycleManager,
		eExceptionManager,
		eLogManager,
		eEnd
	};

	enum class EException {
		eErrorCodeBegin = _LifecycleManager_Id,
		eEventTypeError,
		eSchedulerNotRegistered,
		eComponentNotRegistered,
		eInitializationReplyError,
		eErrorCodeEnd
	};

	class ParamInitializeAsALifecycleManager : public BaseObject {
	private:
		Scheduler* m_pMainScheduler;
	public:
		ParamInitializeAsALifecycleManager(Scheduler* pMainScheduler) {
			m_pMainScheduler = pMainScheduler;
		}
		~ParamInitializeAsALifecycleManager() {
		}
		Scheduler* GetPMainScheduler() {
			return this->m_pMainScheduler;
		}
	};
};
