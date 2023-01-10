#pragma once

#include "../typedef.h"
#define _PTimer_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimer)
#define _PTimer_Name "PTimer"

#include "../../5PPlatform/PComponent/PComponent.h"
#include "../../3Technical/Timer/Timer.h"
#undef GetClassName

#define _SECOND 10000000

class PTimer : public PComponent, public Timer
{
public:
	enum EHandleEvent {
		eQueue, eTimer, eSize
	};
private:
	HANDLE m_hTimer;
	int m_nDueTime;
	int m_nInterval;

public:
	PTimer(
		int nDueTime, int nInterval,
		int nClassId = _PTimer_Id,
		const char* pcClassName = _PTimer_Name);
	virtual ~PTimer();
	virtual void Run();

protected:
	virtual void RegisterEventTypes();
	virtual void RegisterExceptions();

	virtual void Initialize();
	virtual void Finalize();

	virtual void TimeOut();

	virtual void Start();
	virtual void Stop();
};