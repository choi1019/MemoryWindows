//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef COMPONENT
#define COMPONENT "Component"
#include "../TypeDef.h"
#define COMPONENT_ID GETCLASSID(_eComponent_)

#include "../../01Global/Object/Object.h"
#include "../../02Utility/Map/Map.h"
#include "../../03Core/Component/UId.h"
#include "../../03Core/Component/Log.h"
#include "../../03Core/Component/Message.h"
#include "../../03Core/Component/ComponentMsgVector.h"
#include "../../02Utility/String/String.h"

class Component : public Object
{
public:
	typedef _Pair<int, Component*> ComponentPair;
	typedef _Map<int, Component*> TargetComponentMap;
	typedef _Map<int, Component*> ReceiverComponentMap;

	enum EErrorCode {
		eErrorCodeBegin = COMPONENT_ID,
		eNotInitialized,
		eErrorCodeEnd
	};

private:
	// component Id counter
	static unsigned sComponentId;

	// componentName, componentId, taskId
	UId uId;

	// from its parent task
	ComponentMsgVector *pGeneratedMsgVector;  

	// msg receivers and targets
	ReceiverComponentMap receiverComponentMap;
	TargetComponentMap targetComponentMap;

	// associations
	Component *pLifecycleManager;
	Component *pExceptionManager;

public:
	/////////////////////////////////////////////////////////////////////////////////
	// constructors & destructors
	/////////////////////////////////////////////////////////////////////////////////
	Component(const String &name, const ClassInfo &classInfo): 
		pLifecycleManager(NULL), pExceptionManager(NULL), pGeneratedMsgVector(NULL),
		Object(classInfo) 
	{
		this->uId = Make_UId(name, sComponentId++, UNDEFINED);
	}
	Component(String &name, const ClassInfo &classInfo):
		pLifecycleManager(NULL), pExceptionManager(NULL), pGeneratedMsgVector(NULL), 
		Object(classInfo) 
	{
		this->uId = Make_UId(name, sComponentId++, UNDEFINED);
	}
	Component(String &name, int taskId, const ClassInfo &classInfo): 
		pLifecycleManager(NULL), pExceptionManager(NULL), pGeneratedMsgVector(NULL),
		Object(classInfo) 
	{
		this->uId = Make_UId(name, sComponentId++, taskId);
	}
	virtual ~Component() {}

	////////////////////////////////////////////////////
	// initialize before this is allocated and associated
	virtual void initialize() {
		Object::initialize();
		if (this->pLifecycleManager == NULL) {
			throw EErrorCode::eNotInitialized;
		}
		if (this->pExceptionManager == NULL) {
			throw EErrorCode::eNotInitialized;
		}

		this->receiverComponentMap.clear();
		this->targetComponentMap.clear();
	}
	virtual void finalize() {
		Object::finalize();
	}

	////////////////////////////////////////////////////
	// start after this is allocated and associated
	virtual void startAComponent() {
		// pGeneratedMsgVector is not set from its parent Task
		if (this->pGeneratedMsgVector == NULL) {
			throw EErrorCode::eNotInitialized;
		}
		printf("%s::startAComponent\n", this->getComponentName().c_str());
	}
	virtual void stopAComponent() {
		printf("%s::stopAComponent\n", this->getComponentName().c_str());
	}

	/////////////////////////////////////////////////////////////////////////////////
	// getters & setters
	/////////////////////////////////////////////////////////////////////////////////

	inline UId getUId() { return this->uId; }
	inline const String getComponentName() { return this->uId.getComponentName(); }
	inline int getComponentId() { return this->uId.getComponentId(); }
	inline int getTaskId() { return this->uId.getTaskId(); }
	inline void setTaskId(int taskId) { this->uId.setTaskId(taskId); }

	inline TargetComponentMap *getPTargetComponentMap() { return &this->targetComponentMap; }
	// system components are set, when this is associated
	inline void setLifecycleManager(Component *pLifecycleManager) { this->pLifecycleManager = pLifecycleManager; }
	inline Component* getLifecycleManager() { return this->pLifecycleManager; }
	inline void setExceptionManager(Component *pExceptionManager) { this->pExceptionManager = pExceptionManager; }
	inline Component* getExceptionManager() { return this->pExceptionManager; }

	/////////////////////////////////////////////////////////////////////////////////
	// associations
	/////////////////////////////////////////////////////////////////////////////////
	// set from parent task when this component is allocated to it
	inline void setGeneratedMsgVector(ComponentMsgVector *pGeneratedMsgVector) { this->pGeneratedMsgVector = pGeneratedMsgVector; }

	// source & targets
	inline virtual void associateATarget(Component *pTargetComponent) {
		this->targetComponentMap.insert(Make_Pair(pTargetComponent->getComponentId(), pTargetComponent));
	}
	// sender & receivers
	inline virtual void associateAReceiver(Component *pReceiverComponent) {
		this->receiverComponentMap.insert(Make_Pair(pReceiverComponent->getComponentId(), pReceiverComponent));
	}

protected:
	/////////////////////////////////////////////////////////////////////////////////
	// msg handling
	/////////////////////////////////////////////////////////////////////////////////
	inline void addAMsg(Message *pMsg) {
		// trigger 'processAMessage' function of this parent's task
		this->pGeneratedMsgVector->add(pMsg);
	}
	inline void addAReceiverMsg(
		int eventType, const UId &targetUid, int iArg = 0, ValueObject *pArg = NULL, int replyType = NOREPLY) {
		Message *pMsg = new(Message::pMemoryManager) Message(eventType, this->getUId(), targetUid, iArg, pArg, replyType);
		this->addAMsg(pMsg);
	}
	inline void addAReplyMsg(Message *pMsg) {
		Message *pReplyMsg = new(Message::pMemoryManager) Message (
			pMsg->getReplyType(), pMsg->getTargetUId(), pMsg->getSourceUId(), pMsg->getIArg(), pMsg->getPArg(), NOREPLY);
		this->addAMsg(pReplyMsg);
	}
	inline void addATargetMsg(int eventType, int iArg = 0, ValueObject *pArg = NULL, int replyType = NOREPLY) {
		for (ComponentPair componenPair : this->targetComponentMap) {
			this->addAReceiverMsg(eventType, componenPair.second->getUId(), iArg, pArg, replyType);
		}
	}
	virtual void processAMsg(Message *pMsg) {}

public:
	inline void processAMsgEx(Message *pMsg) {
		this->processAMsg(pMsg);
		////////////////////////////////////////////////////////////
		// delete a processed message
		////////////////////////////////////////////////////////////
		try {
			pMsg->destroy(Message::pMemoryManager);
		}
		catch (int error) {
			Log *pLog = new(Log::pMemoryManager) Log(error, this->getUId(), __FUNCTION__, __LINE__);
			this->addAReceiverMsg(Log::mError, this->getExceptionManager()->getUId(), 0, pLog);
		}
	}

	void printAssociations() {
		printf("	  - Target\n");
		for (ComponentPair componentPair : this->targetComponentMap) {
			Component *pComponent = componentPair.second;
			printf("		%s(%d, %d)\n", pComponent->getComponentName().c_str(), pComponent->getTaskId(), pComponent->getComponentId());
		}
		printf("	  - Receiver\n");
		for (ComponentPair componentPair : this->receiverComponentMap) {
			Component *pComponent = componentPair.second;
			printf("		%s(%d, %d)\n", pComponent->getComponentName().c_str(), pComponent->getTaskId(), pComponent->getComponentId());
		}
	}
};
#endif