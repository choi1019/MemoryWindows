#pragma once
#include "../TypeDef.h"
#define EVENTQUEUE "EventQueue"  // class string name
#define EVENTQUEUE_ID GET_MODULE_ID(ECore::eEventQueue)  // class int id

#include "../../../../Core_Lib/Source/00Global/Object/Object.h"
#include "Event.h"

class EventQueue : public Object {
private:
	unsigned schedulerId;
//	Event *buffer[MAXQUEUESIZE];
//	int head, tail, length;
public:
	EventQueue(ClassInfo classInfo = ClassInfo(EVENTQUEUE_ID, EVENTQUEUE), unsigned schedulerId = UNDEFINED):
		Object(classInfo), schedulerId(schedulerId)
	//	, head(0), tail(0), length(0) 
	{
	//	*buffer = { 0 };
	}
	virtual ~EventQueue() {}

	void setSchedulerId(unsigned schedulerId) { this->schedulerId = schedulerId; }
	unsigned getSchedulerId() { return this->schedulerId; }

	virtual Event* pop() = 0;
	/*
	{
		Event* pEvent = buffer[this->tail];
		this->tail = (this->tail+1) % MAXQUEUESIZE;
		this->length--;
		return pEvent;
	}
	*/
	virtual void push(Event* pEvent) = 0;
	/*
	{
		buffer[this->head] = pEvent;
		this->head = (this->head+1) % MAXQUEUESIZE;
		this->length++;
	}
	*/

//	virtual unsigned size() { return this->length; }
};
