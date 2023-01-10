#pragma once

#include "../typedef.h"
#define _SocketClient_Id _GET_CLASS_UID(_ELayer_Technical::_eSocketClient)
#define _SocketClient_Name "Socket"

#include "../../1Base/Object/BaseObject.h"
#include "../../2Platform/EventQueue/Event.h"

class SocketClient {
public:
	////////////////////////////////////////
	// Component Parts
	////////////////////////////////////////
	enum class EPart {
		eBegin = _SocketClient_Id,
		eSocketPart,
		eEnd
	};

	////////////////////////////////////////
	// provided interface
	////////////////////////////////////////
	enum class EGroups {
		eBegin = _SocketClient_Id,
		eGroup1,
		eGroup2,
		eEnd
	};

	// event type
	enum class EEventType {
		eBegin = _SocketClient_Id,
		eEnd
	};
	enum class EError {
		eBegin = _SocketClient_Id,
		eSocketCreationError,
		eSetSocketError,
		eSocketOut,
		eInvalidEvent,
		eEnd
	};

	class ParamStartSocketClient : public BaseObject {
	private:
		int m_nDueTime;
		int m_nInterval;
	public:
		ParamStartSocketClient(int m_nDueTime, int nInterval)
			: m_nDueTime(UNDEFINED)
			, m_nInterval(UNDEFINED)
		{
			this->m_nInterval = nInterval;
		}
		~ParamStartSocketClient() {}

		int GetDueTime() {
			return m_nDueTime;
		}
		int GetInterval() {
			return m_nInterval;
		}
	};

public:
	SocketClient() {}
	~SocketClient() {}

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Run() = 0;
};

