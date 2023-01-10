#pragma once

#include "../../2Platform/Scheduler/Scheduler.h"
#include "../../2Platform/LifecycleManager/LifecycleManager.h"
#include "IMain.h"

class Main : public Scheduler, public IMain
{
private:
	LifecycleManager* m_pLifecycleManager;

public:
	Main(
		unsigned uClassId = _Main_Id,
		const char* pcClassName = _Main_Name);
	virtual ~Main();

	virtual void Run();
	// do nothing as a scheduler
	virtual void InitializeAsAScheduler(int uPriority);
	virtual void Start();
	virtual void Join();

	virtual void Pause();
	virtual void Resume();
	virtual void Stop();

	virtual void FinalizeAsAScheduler();

protected:
	LifecycleManager* GetPLifecycleManager() { return this->m_pLifecycleManager; }
	void SetPLifecycleManager(LifecycleManager* pLifecycleManager) { this->m_pLifecycleManager = pLifecycleManager; }

	virtual void SendStartEvent();

	virtual void InitializeAsAMain(Event* pEvent);
	virtual void FinalizeAsAMain(Event* pEvent);

	virtual void ProcessAEvent(Event* pEvent);
};

