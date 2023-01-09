#pragma once

#include "../typedef.h"
#define _Timer_Id _GET_CLASS_UID(_ELayer_Technical::_eTimer)
#define _Timer_Name "Timer"

#include "../../1Base/Object/BaseObject.h"
#include "../../2Platform/EventQueue/Event.h"

class Timer {
public:
	////////////////////////////////////////
	// Component Parts
	////////////////////////////////////////
	enum class EPart {
		eBegin = _Timer_Id,
		eTimerPart,
		eEnd
	};

	////////////////////////////////////////
	// provided interface
	////////////////////////////////////////
	enum class EGroups {
		eBegin = _Timer_Id,
		eGroup1,
		eGroup2,
		eEnd
	};

	// event type
	enum class EEventType {
		eBegin = _Timer_Id,
		eTimeOut,
		eEnd
	};
	enum class EError {
		eBegin = _Timer_Id,
		eTimerCreationError,
		eSetTimerError,
		eTimerOut,
		eInvalidEvent,
		eEnd
	};

	class ParamStartTimer : public BaseObject {
	private:
		int m_nDueTime;
		int m_nInterval;
	public:
		ParamStartTimer(int m_nDueTime, int nInterval)
			: m_nDueTime(UNDEFINED)
			, m_nInterval(UNDEFINED)
		{
			this->m_nInterval = nInterval;
		}
		~ParamStartTimer() {}

		int GetDueTime() {
			return m_nDueTime;
		}
		int GetInterval() {
			return m_nInterval;
		}
	};

public:
	Timer() {}
	~Timer() {}

	virtual void TimeOut() = 0;
};

