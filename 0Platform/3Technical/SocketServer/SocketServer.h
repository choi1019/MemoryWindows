#pragma once

#include "../typedef.h"
#define _SocketServer_Id _GET_CLASS_UID(_ELayer_Technical::_eSocketServer)
#define _SocketServer_Name "Socket"

#include "../../1Base/Object/BaseObject.h"
#include "../../2Platform/EventQueue/Event.h"

class SocketServer {
public:
	////////////////////////////////////////
	// Component Parts
	////////////////////////////////////////
	enum class EPart {
		eBegin = _SocketServer_Id,
		eSocketPart,
		eEnd
	};

	////////////////////////////////////////
	// provided interface
	////////////////////////////////////////
	enum class EGroups {
		eBegin = _SocketServer_Id,
		eGroup1,
		eGroup2,
		eEnd
	};

	// event type
	enum class EEventType {
		eBegin = _SocketServer_Id,
		eEnd
	};
	enum class EError {
		eBegin = _SocketServer_Id,
		eSocketCreationError,
		eSetSocketError,
		eSocketOut,
		eInvalidEvent,
		eEnd
	};

	class ParamStartSocketServer : public BaseObject {
	private:
		int m_nDueTime;
		int m_nInterval;
	public:
		ParamStartSocketServer(int m_nDueTime, int nInterval)
			: m_nDueTime(UNDEFINED)
			, m_nInterval(UNDEFINED)
		{
			this->m_nInterval = nInterval;
		}
		~ParamStartSocketServer() {}

		int GetDueTime() {
			return m_nDueTime;
		}
		int GetInterval() {
			return m_nInterval;
		}
	};

public:
	SocketServer() {}
	~SocketServer() {}

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Run() = 0;
};

