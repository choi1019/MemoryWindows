#pragma once

#include "../TypeDef.h"
#define ComponentPart_Id _GET_CLASS_UID(_EPlatform::_eComponentPart)
#define ComponentPart_Name "ComponentPart"  // class string name

#include "../../1Base/StdLib/Map.h"
#include "../../1Base/StdLib/Vector.h"
#include "../../1Base/Object/BaseObject.h"

#include "../../2Platform/Component/UId.h"
#include "../../2Platform/EventQueue/Event.h"

#undef GetClassName

class ComponentPart : public BaseObject {
public:
	enum class EState {
		eBegin = ComponentPart_Id,

		eCreated,
		eAllocated,
		eAssociated,
		eTargeted,
		eInitialized,
		eStarted,
		eRunning,
		ePaused,
		eStopped,
		eFinalized,
		eDeleted,

		eEnd
	};

	enum class EException {
		eBegin = ComponentPart_Id,
		eNotAllocated,
		eReceiverNotFound,
		eNotAssociated,
		eNotTargeted,
		eEventNotSupported,
		eEnd
	};

protected:
	//private:
		// UId, Component Id, EventQueue *
	UId* m_pUId;
	EState m_eState;

	// <receiverName, receiverComponent UId>
	Map<unsigned, UId, MAXRECEIVERCOMPONENTS>* m_pmReceivers;
	// <targetGroupName, vector<targetUId>>
	Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* m_pmTargetsGroups;

	// for Sequencing
	Event* m_pSequenceHead;
	Event* m_pSequenceParent;

	void RegisterExceptions();

public:
	// for Component
	ComponentPart(
		unsigned uClassId = ComponentPart_Id,
		const char* acClassName = ComponentPart_Name);
	virtual ~ComponentPart();
	virtual void Initialize();
	virtual void Finalize();

	// getters and setters
	UId* GetPUId() { return m_pUId; }
	UId GetUId() { return *m_pUId; }
	void SetPUId(UId* pUId) { m_pUId = pUId; }
	unsigned GetComponentId() { return m_pUId->GetComponentId(); }
	void SetComponentId(unsigned uComponentId) { return m_pUId->SetComponentId(uComponentId); }
	EState GetEState() { return this->m_eState; }

	Map<unsigned, UId, MAXRECEIVERCOMPONENTS>* GetPMReceivers() { return m_pmReceivers; }
	void SetPMReceivers(Map<unsigned, UId, MAXRECEIVERCOMPONENTS>* pmReceivers) { this->m_pmReceivers = pmReceivers; }
	Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* GetPMTargetsGroups() { return this->m_pmTargetsGroups; }
	void SetPMTargetsGroups(Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* pmTargetsGroups) { m_pmTargetsGroups = pmTargetsGroups; }

	// methods
	UId FindUid(int nReceiverName);

	// for sequencing events
	void BeginSequence(Event* pEvent);
	void EndSequence();

protected:
	///////////////////////////////////////
	// prepare event
	///////////////////////////////////////
	void SendAEvent(Event* pEvent);
	///////////////////////////////////////
	// reply event
	///////////////////////////////////////
	void PrepareReplyEvent(Event* pEvent);

	///////////////////////////////////////
	// reply event
	///////////////////////////////////////
	void SendReplyEvent(UId uIdTarget, int nEventType, long long lArg = 0, BaseObject* pArg = nullptr
		, int nReplyType = UNDEFINED);
	void SendReplyEvent(int nReceiverName, int nEventType, long long lArg = 0, BaseObject* pArg = nullptr
		, int nReplyType = UNDEFINED);

	///////////////////////////////////////
	// no reply event
	///////////////////////////////////////
	void SendNoReplyEvent(UId uIdTarget, int nEventType, long long lArg = 0, BaseObject* pArg = nullptr);
	void SendNoReplyEvent(int nReceiverName, int nEventType, long long lArg = 0, BaseObject* pArg = nullptr);
	void SendNoReplyEventLast(UId uIdTarget, int nEventType, long long lArg = 0, BaseObject* pArg = nullptr);
	void SendNoReplyEventLast(int nReceiverName, int nEventType, long long lArg = 0, BaseObject* pArg = nullptr);

	///////////////////////////////////////
	// target events
	///////////////////////////////////////
	void SendTargetEvents(unsigned groupName, unsigned eventType, long long lArg = 0, BaseObject* pArg = nullptr);
	void SendTargetEventsLast(unsigned groupName, unsigned eventType, long long lArg = 0, BaseObject* pArg = nullptr);

};