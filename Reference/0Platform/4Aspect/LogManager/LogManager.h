#pragma once

#include "../typedef.h"
#define _LogManager_Id _GET_CLASS_UID(_ELayer_Aspect::_eLogManager)
#define _LogManager_Name "LogManager"

#include "../../2Platform/Component/Component.h"
#include "../../1Base/Aspect/Log.h"

class LogManager : public Component
{
public:
	// interface
	enum class EEventType {
		eBegin = _LogManager_Id,
		eLog,
		eEnd
	};
public:
	// contructors and destructors
	LogManager(
		int nClassId = _LogManager_Id,
		const char* pcClassName = _LogManager_Name)
		: Component(nClassId, pcClassName)
	{
	}
	virtual ~LogManager() 
	{
	}

protected:
	virtual void Initialize() {
		Component::Initialize();
	}
	virtual void Finalize() {
		Component::Finalize();
	}

	virtual void Record(Log* pLog) {

	}

private:
	void Record(Event* pEvent) {
		Log* pLog = (Log *)pEvent->GetPArg();
		this->Record(pLog);
	}

public:
	virtual void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (int)EEventType::eLog:
			this->Record(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};

