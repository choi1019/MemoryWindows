#pragma once

#include "../typedef.h"
#define _PScheduler_Id _GET_CLASS_UID(_EPPlatform::_ePScheduler)
#define _PScheduler_Name "PScheduler"

#include "../../2Platform/Scheduler/Scheduler.h"
#include "../PEventQueue/PEventQueue.h"

#undef GetClassName

#define PRIORITY 100
DWORD WINAPI _ThreadProc(LPVOID lpParam);

class PScheduler : public Scheduler
{
private:
	HANDLE m_hThread;
	DWORD m_dwThreadId;

public:
	PScheduler(
		int uClassId = _PScheduler_Id,
		const char* pcClassName = _PScheduler_Name);
	virtual ~PScheduler();

	virtual void InitializeAsAScheduler(int uPriority);
	virtual void FinalizeAsAScheduler();

	virtual void Start();
	// mutual exclusion /////////////////////////////////////////
	virtual void Join();

protected:
	virtual void Initialize();
	virtual void Finalize();

//	void RunOnce();
};