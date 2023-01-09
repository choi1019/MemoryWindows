#pragma once

#include "../TypeDef.h"
#define UId_Id _GET_CLASS_UID(_EPlatform::_eUId)
#define UId_Nname "UId"

#include "../../1Base/Object/BaseObject.h"

class EventQueue;
class UId: public BaseObject {
private:
	unsigned m_uComponentId;
	EventQueue* m_pEventQueue;

public:
	UId(unsigned uComponentId = UNDEFINED, EventQueue* pEventQueue = nullptr)
		: BaseObject(UId_Id, UId_Nname)
		, m_uComponentId(uComponentId)
		, m_pEventQueue(pEventQueue)
	{}
	virtual ~UId() {}

	virtual void Initialize() {
		BaseObject::Initialize();
	}
	virtual void Finalize() {
		BaseObject::Finalize();
	}

	void SetPEventQueue(EventQueue* pEventQueue) { this->m_pEventQueue = pEventQueue; }
	EventQueue* GetPEventQueue() { return this->m_pEventQueue; }

	int GetComponentId() { return this->m_uComponentId; }
	void SetComponentId(unsigned componentId) { this->m_uComponentId = componentId; }

	bool operator==(UId& other) {
		return (this->m_pEventQueue == other.m_pEventQueue && this->m_uComponentId == other.m_uComponentId);
	}
};
