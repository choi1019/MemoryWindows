#pragma once
#include "../TypeDef.h"
#define EVENT "Event"  // class string name
#define EVENT_ID GET_MODULE_ID(ECore::eEvent)  // class int id

#include "../../../../Core_Lib/Source/00Global/ValueObject/ValueObject.h"
#include "../../../../Core_Lib/Source/03Core/Component/UId.h"

class Event : public ValueObject {
private:
	static int gCounter;

	unsigned id;
	UId sourceUId;
	UId targetUId;
	unsigned type;
	int iArg;
	ValueObject* pArg;
public:
	static HeapDynamic *spMemoryManager;

	Event(UId sourceUId, UId targetUId, unsigned type, int iArg = 0, ValueObject* pArg = nullptr) :
		ValueObject(ClassInfo(EVENT_ID, EVENT))
	{
		this->id = gCounter++;
		this->targetUId = targetUId; 
		this->sourceUId = sourceUId;
		this->type = type;
		this->iArg = iArg;
		this->pArg = pArg;
	}
	~Event() {}

	unsigned getId() { return this->id; }
	UId getSourceUId() { return this->sourceUId; }
	UId getTargetUId() { return this->targetUId; }
	unsigned getType() { return this->type; }
	int getIArg() { return this->iArg; }
	ValueObject* getPArg() { return this->pArg; }
};

