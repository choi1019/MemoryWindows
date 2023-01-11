#pragma once
#include "../TypeDef.h"
#define COMPONENTPART "ComponentPart"  // class string name
#define COMPONENTPART_ID GET_MODULE_ID(ECore::eComponentPart)  // class int id

#include "UId.h"
#include "../../../../Core_Lib/Source/00Global/Object/Object.h"
#include "../../../../Core_Lib/Source/03Core/EventQueue/Event.h"
#include "../../../../Core_Lib/Source/03Core/EventQueue/EventQueue.h"
#include "../../../../Core_Lib/Source/01Utility/collection/map.h"
#include "../../../../Core_Lib/Source/01Utility/collection/vector.h"

class ComponentPart : public Object {
public:
	// for Component
	ComponentPart(ClassInfo classInfo = ClassInfo(COMPONENTPART_ID, COMPONENTPART)) :
		pUId(nullptr), pReceiversMap(nullptr), pTargetsGroupMap(nullptr),
		Object(classInfo)
	{}
	virtual ~ComponentPart() {}

	void setPUId(UId* pUId) { this->pUId = pUId; }
	void setReceiversMap(map<unsigned, UId, MAXRECEIVERCOMPONENTS>* pReceiversMap) { this->pReceiversMap = pReceiversMap; }
	void setTargetGroupMap(map<unsigned, vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* pTargetsGroupMap) { this->pTargetsGroupMap = pTargetsGroupMap; }

protected:
	void SendAEvent(Event* pEvent) {
		pEvent->getTargetUId().getPEventQueue()->push(pEvent);
	}
	void SendAEvent(UId targetUId, unsigned eventType, int iArg = 0, ValueObject* pArg = nullptr) {
		Event* pEvent = new(Event::spMemoryManager) Event(*this->pUId, targetUId, eventType, iArg, pArg);
		this->SendAEvent(pEvent);
	}
	void SendAEvent(unsigned receiverName, unsigned eventType, int iArg = 0, ValueObject* pArg = nullptr) {
		this->SendAEvent(this->pReceiversMap->find(receiverName)->second, eventType, iArg, pArg);
	}

	void SendAReplyEvent(Event* pEvent) {
		Event* pReplyEvent = new(Event::spMemoryManager) Event(*this->pUId, pEvent->getSourceUId(), pEvent->getType() + 1, pEvent->getIArg(), pEvent->getPArg());
		this->SendAEvent(pReplyEvent);
	}

	void SendTargetEvents(unsigned groupName, unsigned eventType, int iArg = 0, ValueObject* pArg = nullptr) {
		for (auto itr : *(this->pTargetsGroupMap->find(groupName)->second)) {
			this->SendAEvent(itr, eventType, iArg, pArg);
		}
	}

	// inherited properties from aggreation root
	void setPartPReceiversMap(map<unsigned, UId, MAXRECEIVERCOMPONENTS>* pReceiversMap) { this->pReceiversMap = pReceiversMap; }
	void setPartPTargetsGroupMap(map<unsigned, vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* pTargetsGroupMap) { this->pTargetsGroupMap = pTargetsGroupMap; }

private:
	UId* pUId;

	// <receiverName, receiverComponent UId>
	map<unsigned, UId, MAXRECEIVERCOMPONENTS>* pReceiversMap;
	// <targetGroupName + vector<targetUId>>
	map<unsigned, vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* pTargetsGroupMap;
};
