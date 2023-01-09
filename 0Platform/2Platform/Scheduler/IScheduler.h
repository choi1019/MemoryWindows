#pragma once

#include "../typedef.h"
#define _Scheduler_Id _GET_CLASS_UID(_EPlatform::_eScheduler)
#define _Scheduler_Name "Scheduler"

#include "../Component/Component.h"

class IScheduler
{
public:
	/// <summary>
	/// Required Interface
	/// </summary>
	enum class EEventType {
		eBegin = _Scheduler_Id,
		// create a thread
		eInitializeAsAScheduler,
		eFinalizeAsAScheduler,

		eAllocateAComponent,
		eDellocateAComponent,

		eStart,
		eIsStarted,
		ePause,
		eResume,
		eStop,
		eEnd
	};

	enum class EState {
		eBegin,
		eCreated,
		eInitializedAsAScheduler,
		eStarted,
		ePaused,
		eStopped,
		eFinalizedAsAScheduler,
		eEnd
	};

	enum class EError {
		eBegin = _Scheduler_Id,
		// create a thread
		eComponentNotFound,
		eNullPoint,
		eThreadCreationFailed,
		eThreadTerminationFailed,
		eEnd

	};

	class ParamAllocateAComponent : public BaseObject {
	private:
		Component* m_pComponentAllocated;
	public:
		ParamAllocateAComponent(Component* pComponentAllocated = nullptr)
			: m_pComponentAllocated(pComponentAllocated)
		{
		}
		~ParamAllocateAComponent() {
		}
		Component* GetPComponentAllocated() { return this->m_pComponentAllocated; }
		void SetPComponentAllocated(Component* pComponentAllocated) { this->m_pComponentAllocated = pComponentAllocated; }
	};
};

