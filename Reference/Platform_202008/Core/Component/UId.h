#pragma once
#include "../TypeDef.h"
#define UID "UId"  // class string name
#define UID_ID GET_MODULE_ID(ECore::eUId)  // class int id

#include "../../../../Core_Lib/Source/00Global/ValueObject/ValueObject.h"

class EventQueue;
class UId: public ValueObject {
private:
	EventQueue* pEventQueue;
	unsigned componentId;
public:
	static HeapDynamic *spMemoryManager;

	UId(): pEventQueue(nullptr), componentId(UNDEFINED), 
		ValueObject(ClassInfo(UID_ID, UID)) {}
	UId(int componentId): pEventQueue(nullptr), componentId(componentId), 
		ValueObject(ClassInfo(UID_ID, UID)) {}
	UId(EventQueue* pEventQueue, unsigned componentId) : pEventQueue(pEventQueue), componentId(componentId), 
		ValueObject(ClassInfo(UID_ID, UID)) {}
	~UId() {}

	void setPEventQueue(EventQueue* pEventQueue) { this->pEventQueue = pEventQueue; }
	EventQueue* getPEventQueue() { return this->pEventQueue; }
	int getComponentId() { return this->componentId; }
	void setComponentId(unsigned componentId) { this->componentId = componentId; }
	bool operator==(UId& other) {
		return (this->pEventQueue == other.pEventQueue && this->componentId == other.componentId);
	}
};
