#pragma once

#include "../typeDef.h"
#define _EventQueue_Id _GET_CLASS_UID(_EPlatform::_eEventQueue)
#define _EventQueue_Name "EventQueue"

#include "../../1Base/Object/BaseObject.h"
#include "../../2Platform/Component/IComponent.h"
#include "Event.h"

class EventQueue : public BaseObject
{
private:
	int m_nSchedulerId;
	Event* m_pFront;
	Event* m_pRear;
	int m_nLength;

public:
	int GetSchedulerId() { return this->m_nSchedulerId; }

	EventQueue(
		int uSchedulerId = UNDEFINED,
		int uClassId = _EventQueue_Id,
		const char* pcClassName = _EventQueue_Name)
		: BaseObject(uClassId, pcClassName)
		, m_nSchedulerId(uSchedulerId)
		, m_pFront(nullptr)
		, m_pRear(nullptr)
		, m_nLength(0)
	{
	}
	virtual ~EventQueue() 
	{
	}
	virtual void Initialize() {
		BaseObject::Initialize();
	}
	virtual void Finalize() {
		BaseObject::Finalize();
	}

	void ShowState(const char* pcMessage) {
		LOG_HEADER(pcMessage);
		//		Log("SizeThis + Size Allocated", this->GetSzThis(), m_szSlot * m_uCountTotalSlots).Println();
		//		Log("SizeSlot x CountSlots", m_szSlot, m_uCountTotalSlots).Println();
		unsigned index = 0;
		for (Event* pEvent = m_pFront; pEvent != nullptr; pEvent = pEvent->GetPQueueNext()) {
			LOG("Event",
				pEvent->GetId(),
				Directory::s_dirComponents[pEvent->GetUIdTarget().GetComponentId()],
				Directory::s_dirEvents[pEvent->GetType()]
			);
		}
		LOG_FOOTER(pcMessage, m_nLength);
	}

	virtual void PushFront(Event* pEvent) {
		if (m_nLength == 0) {
			this->m_pRear = pEvent;
			this->m_pFront = pEvent;
		}
		else {
			pEvent->SetPQueueNext(this->m_pFront);
			this->m_pFront = pEvent;
		}
		this->m_nLength++;
	}

	virtual void PushBack(Event* pEvent) {
		if (m_nLength == 0) {
			this->m_pRear = pEvent;
			this->m_pFront = pEvent;
		}
		else {
			this->m_pRear->SetPQueueNext(pEvent);
			this->m_pRear = this->m_pRear->GetPQueueNext();
		}
		this->m_nLength++;
	}
	
	virtual Event* Front() {
		return this->m_pFront;
	}

	virtual Event* Pop(){
		if (m_nLength == 0) {
			return nullptr;
		}
		Event* pEvent = this->m_pFront;
		this->m_pFront = this->m_pFront->GetPQueueNext();
		this->m_nLength--;
		return pEvent;
	}

	virtual bool IsEmpty() {
		return (m_nLength == 0) ? true : false;
	}
};

