#pragma once

#include "../typeDef.h"
#define _Event_Id _GET_CLASS_UID(_EPlatform::_eEvent)
#define _Event_Name "Event"

#include "../../1Base/Aspect/Directory.h"
#include "../../2Platform/Component/UId.h"
#include "../../1Base/Object/BaseObject.h"

class Event: public BaseObject
{
public:
	enum class ESyncType {
		eSync, eAsync
	};
	enum class ESendOrder {
		eFirst, eLast
	};

public:
	static IMemory* s_pMemory;
	void* operator new (size_t size) throw() {
		return s_pMemory->SafeMalloc(size, _Event_Name);
	}
	void operator delete(void* pObject) throw() {
		s_pMemory->SafeFree(pObject);
	}

private:
	static unsigned s_uCounter;

	int m_nId;
	UId m_uidSource;
	UId m_uidTarget;
	int m_nType;
	long long m_lArg;
	BaseObject* m_pPArg;

	int m_nReplyType;
	Event* m_pParentEvent;

	ESyncType m_eSyncType;
	ESendOrder m_eSendOrder;
	bool m_bReply;

	Event* m_pNextSequence;
	Event* m_pQueueNext;
	int m_nCountChildren;

public:
	Event(
		UId uidSource,
		UId uidTarget,
		int nType,
		long long lArg,
		BaseObject* pArg,
		int nReplyType = UNDEFINED,
		Event* pParentEvent = nullptr,
		ESyncType eSyncType = ESyncType::eAsync,
		ESendOrder eSendOrder = ESendOrder::eFirst,

		unsigned uClassId = _Event_Id,
		const char* pcClassName = _Event_Name
	)
		: BaseObject(uClassId, pcClassName)
		, m_nId(s_uCounter++)
		, m_uidSource(uidSource)
		, m_uidTarget(uidTarget)
		, m_nType(nType)
		, m_lArg(lArg)
		, m_pPArg(pArg)
		, m_nReplyType(nReplyType)
		, m_pParentEvent(pParentEvent)
		, m_eSyncType(eSyncType)
		, m_eSendOrder(eSendOrder)

		, m_bReply(false)
		, m_pNextSequence(nullptr)
		, m_pQueueNext(nullptr)
		, m_nCountChildren(0)
	{
	}

	Event(
		const Event& event,
		unsigned uClassId = _Event_Id,
		const char* pcClassName = _Event_Name
	)
		: BaseObject(uClassId, pcClassName)
		, m_nId(s_uCounter++)
		, m_uidSource(event.m_uidSource)
		, m_uidTarget(event.m_uidTarget)
		, m_nType(event.m_nType)
		, m_lArg(event.m_lArg)
		, m_pPArg(m_pPArg == nullptr ? nullptr : event.m_pPArg->Clone())
		, m_nReplyType(event.m_nReplyType)
		, m_pParentEvent(event.m_pParentEvent)
		, m_eSyncType(event.m_eSyncType)
		, m_eSendOrder(event.m_eSendOrder)

		, m_bReply(false)
		, m_pNextSequence(event.m_pNextSequence)
		, m_pQueueNext(event.m_pQueueNext)
		, m_nCountChildren(event.m_nCountChildren)
	{
	}
	virtual ~Event() {
		if (m_pPArg != nullptr) {
			delete m_pPArg;
		}
	}
	virtual void Initialize() {
		BaseObject::Initialize();
	}
	virtual void Finalize() {
		BaseObject::Finalize();
	}

	int GetId() { return this->m_nId; }
	UId GetUIdSource() { return this->m_uidSource; }
	UId GetUIdTarget() { return this->m_uidTarget; }
	int GetType() { return this->m_nType; }
	long long GetlArg() { return this->m_lArg; }
	BaseObject* GetPArg() { return this->m_pPArg; }

	void SetUIdSource(UId uidSource) { this->m_uidSource = uidSource; }
	void SetUIdTarget(UId uidTarget) { this->m_uidTarget = uidTarget; }
	void SetType(int nType) { this->m_nType = nType; }
	void SetlArg(long long lArg) { this->m_lArg = lArg; }
	void SetPArg(BaseObject* pPArg) { this->m_pPArg = pPArg; }

	int GetReplyType() { return this->m_nReplyType; }
	void SetReplyType(int nReplyType) { this->m_nReplyType = nReplyType; }
	Event* GetPParentEvent() { return this->m_pParentEvent; }
	void SetPParentEvent(Event* pParentEvent) { this->m_pParentEvent = pParentEvent; }

	void SetESyncType(ESyncType eSyncType) {	this->m_eSyncType = eSyncType; }
	ESyncType GetESyncType() { return this->m_eSyncType; }
	void SetESendOrder(ESendOrder eSendOrder) { this->m_eSendOrder = eSendOrder; }
	ESendOrder GetESendOrder() { return this->m_eSendOrder; }
	bool IsReply() { return this->m_bReply; }
	void SetBReply(bool bReply) { this->m_bReply = bReply; }

	// for child count
	void IncrementCountChildren() { this->m_nCountChildren++; }
	void DecrementCountChildren() { this->m_nCountChildren--; }
	int GetCoundChildren() { return this->m_nCountChildren; }
	void SetCountChildren(int nCountChildren) { this->m_nCountChildren = nCountChildren; }

	bool IsAllReplied() {
		if (this->m_pParentEvent == nullptr) {
			return true;
		}
		if (this->m_pParentEvent->GetCoundChildren() > 0) {
			return false;
		}
		return true;
	}

	// for Sequencing
	Event* GetPNextSequence() { return this->m_pNextSequence; }
	void SetPNextSequence(Event* pNextSequence) { this->m_pNextSequence = pNextSequence; }
	// for event queue
	Event* GetPQueueNext() { return this->m_pQueueNext; }
	void SetPQueueNext(Event* pQueueNext) { this->m_pQueueNext = pQueueNext; }

};