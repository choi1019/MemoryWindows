#pragma once
#include "../TypeDef.h"
#define COMPONENT "Component"  // class string name
#define COMPONENT_ID GET_MODULE_ID(ECore::eComponent)  // class int id

#include "ComponentPart.h"

class Component : public ComponentPart {
public:
	enum class EEventType {
		eBegin = COMPONENT_ID,
		eInitialize, eInitializeReply,
		eAssociateAReceiver, eAssociateAReceiverReply,
		eAssociateTargets, eAssociateTargetsReply,
		eFinalize, eFinalizeReply,
		eEnd
	};
	enum class EReceivers {
		eBegin = COMPONENT_ID,
		eThis,
		eLifecycleManager,
		eDisplay,
		eEnd
	};
	enum class EAttributes {
		eBegin = COMPONENT_ID,
		eEnd
	};

	enum class EExceptions {
		eBegin = COMPONENT_ID,
		eEnd
	};

public:
	Component(ClassInfo classInfo = ClassInfo(COMPONENT_ID, COMPONENT)) :
		ComponentPart(classInfo) 
	{
		this->uId.setComponentId(gCounter++);

		this->setPUId(&this->uId);
		this->setPartPReceiversMap(&this->receiversMap);
		this->setPartPTargetsGroupMap(&this->targetsGroupMap);
	}
	virtual ~Component() {}

	UId& getUId() { return this->uId; }
	unsigned getComponentId() { return this->uId.getComponentId(); }

	void addPart(ComponentPart* pComponentPart) {
		this->componentPartVector.push_back(pComponentPart);
	}

	void allocate(EventQueue* pEventQueue) {
		this->uId.setPEventQueue(pEventQueue);
		// to send a event to self
		this->associateAReceiver((unsigned)EReceivers::eThis, this->getUId());

		for (auto itr : this->componentPartVector) {
			itr->setPUId(&(this->uId));
			itr->setReceiversMap(&this->receiversMap);
			itr->setTargetGroupMap(&this->targetsGroupMap);
		}
	}

	virtual void processAEvent(Event* pEvent) {
		switch ((EEventType)(pEvent->getType())) {
		case EEventType::eInitialize:
			this->initialize(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eFinalize:
			this->finalize(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eAssociateAReceiver:
			this->associateAReceiver(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		case EEventType::eAssociateTargets:
			this->associateTargets(pEvent);
			this->SendAReplyEvent(pEvent);
			break;
		default:
//			cout << _methodName() << "() => UnProcessed Event \n";
			break;
		}
	}

protected:
	virtual void initialize() = 0;
	virtual void finalize() = 0;

	void associateAReceiver(unsigned receiverName, UId receiverUId) {
		this->receiversMap[receiverName] = receiverUId;
	}
	void associateTargets(unsigned groupName, vector<UId>* pNewTargetUIdVector) {
		// find a targetUIdVector
		auto itrTargetGroup = this->targetsGroupMap.find(groupName);
		if (itrTargetGroup == this->targetsGroupMap.end()) {
			this->targetsGroupMap[groupName] = new vector<UId, MAXTARGETCOMPONENTS>();
		}
		for (auto newTargetUId : *pNewTargetUIdVector) {
			bool bExist = false;
			for (UId targetUId : *(this->targetsGroupMap[groupName])) {
				if (targetUId == newTargetUId) {
					// arleady exist
					bExist = true;
					break;
				}
			}
			if (!bExist) {
				this->targetsGroupMap[groupName]->push_back(newTargetUId);
				/*
					cout << _methodName()
					<< "("
					<< groupName
					<< ", ("
					<< newTargetUId.getPEventQueue()->getSchedulerId()
					<< ", "
					<< newTargetUId.getComponentId()
					<< "))\n";
					*/
			}
		}
		delete(pNewTargetUIdVector);
	}

private:
	/////////////////////////////////////////////////////////////
	// component id counter
	static int gCounter;
	UId uId;

	vector<ComponentPart*, MAXCOMPONENTPARTS> componentPartVector;

	// <receiverName, receiverComponent UId>
	map<unsigned, UId, MAXRECEIVERCOMPONENTS> receiversMap;
	// <targetGroupName + vector<targetUId>>
	map<unsigned, vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS> targetsGroupMap;
	/////////////////////////////////////////////////////////////
	// event mapping functions
	/////////////////////////////////////////////////////////////
	void initialize(Event* pEvent) {
		this->initialize();
//		cout << _methodName() << "(pEvent)\n";
	}
	void finalize(Event* pEvent) {
		this->finalize();

		this->receiversMap.clear();
		for (auto itr : targetsGroupMap) {
			delete (itr.second);
		}
		this->targetsGroupMap.clear();
//		cout << _methodName() << "(pEvent)\n";
	}

	void associateAReceiver(Event* pEvent) {
		unsigned receiverName = pEvent->getIArg();
		UId *pReceiverUId = (UId*)pEvent->getPArg();
		this->associateAReceiver(receiverName, *pReceiverUId);

		// delete memory
		pReceiverUId->destroy(UId::spMemoryManager);
	}

	void associateTargets(Event* pEvent) {
		unsigned groupName = pEvent->getIArg();
		vector<UId> *pNewTargetUIdVector = (vector<UId>*)pEvent->getPArg();
		this->associateTargets(groupName, pNewTargetUIdVector);
	}

};
