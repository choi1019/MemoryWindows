#pragma once

#include "../../2Platform/EventQueue/EventQueue.h"
#include "IScheduler.h"

#undef GetClassName

class Scheduler : public Component, public IScheduler
{
private:
	typedef Map<unsigned, Component*> ComponentMap;
	typedef Map<unsigned, Component*>::Iterator MapIterator;
	ComponentMap m_mComponents;

	EventQueue* m_pEventQueue;
	IScheduler::EState m_eState;

	virtual void InitializeVarialbes();
	virtual void DeleteVarialbes();

	void RegisterEventTypes();

public:
	Scheduler(unsigned classId = _Scheduler_Id, const char* className = _Scheduler_Name);
	virtual ~Scheduler();

protected:
	virtual void Initialize();
	virtual void Finalize();

public:
	EventQueue* GetPEventQueue();
	void SetPEventQueue(EventQueue* pEventQueue);

	// jobs to do before the thread is started
	virtual void InitializeAsAScheduler(int uPriority);
	virtual void FinalizeAsAScheduler();

	virtual void Start() = 0;
	virtual void Run();
	virtual void Join() = 0;

protected:
	virtual void RunOnce();

	virtual void Pause();
	virtual void Resume();
	virtual void Stop();

	virtual void IsStarted(Event* pEvent);
	virtual void Pause(Event* pEvent);
	virtual void Resume(Event* pEvent);
	virtual void Stop(Event* pEvent);

	void AllocateAComponent(Component* pComponent);
	void DellocateAComponent(Component* pComponent);

private:
	/// Event Mapping Functions: Unmarshalling
	void InitializeAsAScheduler(Event* pEvent);
	void FinalizeAsAScheduler(Event* pEvent);

	void AllocateAComponent(Event* pEvent);
	void DellocateAComponent(Event* pEvent);

public:
	virtual void ProcessAEvent(Event* pEvent);

};

