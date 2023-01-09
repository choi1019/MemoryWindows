#pragma once

#include "ComponentPart.h"
#include "IComponent.h"

class Component : public IComponent, public ComponentPart
{
private:
//	static unsigned s_uCounter;
	Map<int, ComponentPart*, MAXCOMPONENTPARTS> m_mComponentParts;

public:
	Component(unsigned uClassId = Component_Id, const char* pcClassName = Component_Name);
	virtual ~Component();
	void AddPart(unsigned uName, ComponentPart* pComponentPart);
	ComponentPart* GetPart(unsigned uName);
	void SetPEventQueue(EventQueue* pEventQueue);
public:
	virtual void Run();
	virtual void Start();

protected:
	virtual void RegisterEventTypes();
	virtual void RegisterExceptions();

	virtual void AssociateAReceiver(unsigned receiverName, UId receiverUId);
	virtual void AssociateATarget(unsigned uGroupName, Vector<UId>& vNewUIdTargetComponents);

	virtual void Initialize();
	virtual void Finalize();

	virtual void Stop();
	virtual void Pause();


protected:
	virtual void AssociateAReceiver(Event* pEvent);
	virtual void AssociateATarget(Event* pEvent);

	virtual void Initialize(Event* pEvent);
	virtual void Finalize(Event* pEvent);

	virtual void Start(Event* pEvent);
	virtual void Stop(Event* pEvent);
	virtual void Run(Event* pEvent);
	virtual void Pause(Event* pEvent);

public:
	virtual void ProcessAEvent(Event* pEvent);
};
