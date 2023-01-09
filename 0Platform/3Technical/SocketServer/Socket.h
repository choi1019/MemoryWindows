#pragma once

#include "../typedef.h"
#define _Socket_Id _GET_CLASS_UID(_ELayer_Technical::_eSocket)
#define _Socket_Name "Socket"

#include "../../1Base/Object/BaseObject.h"
#include "../../2Platform/EventQueue/Event.h"

class Socket {
public:
	////////////////////////////////////////
	// Component Parts
	////////////////////////////////////////
	enum class EPart {
		eBegin = _Socket_Id,
		eSocketPart,
		eEnd
	};

	////////////////////////////////////////
	// provided interface
	////////////////////////////////////////
	enum class EGroups {
		eBegin = _Socket_Id,
		eGroup1,
		eGroup2,
		eEnd
	};

	// event type
	enum class EEventType {
		eBegin = _Socket_Id,
		eStart,
		eStop,

		eRun,
		eEnd
	};
	enum class EError {
		eBegin = _Socket_Id,
		eSocketCreationError,
		eSetSocketError,
		eSocketOut,
		eInvalidEvent,
		eEnd
	};

	class ParamStartSocket : public BaseObject {
	private:
		int m_nDueTime;
		int m_nInterval;
	public:
		ParamStartSocket(int m_nDueTime, int nInterval)
			: m_nDueTime(UNDEFINED)
			, m_nInterval(UNDEFINED)
		{
			this->m_nInterval = nInterval;
		}
		~ParamStartSocket() {}

		int GetDueTime() {
			return m_nDueTime;
		}
		int GetInterval() {
			return m_nInterval;
		}
	};

public:
	Socket() {}
	~Socket() {}

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Run() = 0;
};

