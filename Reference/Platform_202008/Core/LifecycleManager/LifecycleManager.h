#pragma once
#include "../TypeDef.h"
#define LIFECYCLEMANAGER "LifecycleManager"  // class string name
#define LIFECYCLEMANAGER_ID GET_MODULE_ID(EApplication::eLifecycleManager)  // class int id

#include "../../../../Core_Lib/Source/03Core/Component/Component.h"
#include "../../../../Core_Lib/Source/03Core/Scheduler/Scheduler.h"
#include "../../../../Core_Lib/Source/04Aspect/BasicIO/Display.h"

class LifecycleManager : public Component {
public:
	enum class EEventType {
		eBegin = LIFECYCLEMANAGER_ID,
		eInitializeAsALifecycleManager, eInitializeAsALifecycleManagerReply,
		eRegisterSchedulers, eRegisterSchedulersReply,
		eRegisterComponents, eRegisterComponentsReply,
		eAllocateComponents, eAllocateComponentsReply,
		eAssociateSendersNReceivers, eAssociateSendersNReceiversReply,
		eAssociateSourcesNTargets, eAssociateSourcesNTargetsReply,
		eInitializeComponents, eInitializeComponentsReply,

		eFinalizeAsALifecycleManager, eFinalizeAsALifecycleManagerReply,
		eEnd
	};

	enum class EReceivers {
		eBegin = LIFECYCLEMANAGER_ID,
		eMain,
		eLifecycleManager,
		eDisplay,
		eEnd
	};

	virtual void processAEvent(Event* pEvent) {
		switch (pEvent->getType()) {
		case (unsigned)EEventType::eInitializeAsALifecycleManager:
			this->initializeAsALifecycleManager(pEvent);
			break;
		case (unsigned)EEventType::eRegisterSchedulers:
			this->registerSchedulers(pEvent);
			break;
		case (unsigned)Scheduler::EEventType::eInitializeAsASchedulerReply:
			this->registerSchedulersReply(pEvent);
			break;
		case (unsigned)EEventType::eRegisterComponents:
			this->registerComponents(pEvent);
			break;
		case (unsigned)EEventType::eAllocateComponents:
			this->allocateComponents(pEvent);
			break;
		case (unsigned)Scheduler::EEventType::eAllocateAComponentReply:
			this->allocateComponentsReply(pEvent);
			break;
		case (unsigned)EEventType::eAssociateSendersNReceivers:
			this->associateSendersNReceivers(pEvent);
			break;
		case (unsigned)Component::EEventType::eAssociateAReceiverReply:
			this->associateSendersNReceiversReply(pEvent);
			break;
		case (unsigned)EEventType::eAssociateSourcesNTargets:
			this->associateSourcesNTargets(pEvent);
			break;
		case (unsigned)Component::EEventType::eAssociateTargetsReply:
			this->associateSourcesNTargetsReply(pEvent);
			break;
		case (unsigned)EEventType::eInitializeComponents:
			this->initializeComponents(pEvent);
			break;
		case (unsigned)Component::EEventType::eInitializeReply:
			this->initializeComponentsReply(pEvent);
			break;
		case (unsigned)EEventType::eInitializeAsALifecycleManagerReply:
			this->initializeAsALifecycleManagerReply(pEvent);
			break;

		case (unsigned)EEventType::eFinalizeAsALifecycleManager:
			this->finalizeAsALifecycleManager(pEvent);
			break;
		default:
			Component::processAEvent(pEvent);
			break;
		}
	}

public:
	LifecycleManager(ClassInfo classInfo = ClassInfo(LIFECYCLEMANAGER_ID, LIFECYCLEMANAGER)) :
		Component(classInfo),
		countComponents(0),
		countAspects(0),
		countSchedulers(0),
		countAllocatedComponents(0),
		countSendersNReceivers(0),
		countSourcesNTargets(0)
	{
	}
	virtual ~LifecycleManager() {}

	virtual void initialize() {}
	virtual void finalize() {}

protected:
	virtual void createTasks() {
		for (auto itr : this->schedulerMap) {
			itr.second->createATask();
		}
	}
	virtual void startTasks() {
		for (auto itr : this->schedulerMap) {
			itr.second->startATask();
		}
	}

	void registerAScheduler(unsigned name, Scheduler* pScheduler) {
		this->schedulerMap[name] = pScheduler;
		this->registerAComponent(name, pScheduler);
//		printf("%s (%s::%d)\n", __func__, pScheduler->getClassInfo().getName(), pScheduler->getComponentId());
	}
	void registerAComponent(unsigned name, Component* pComponent) {
		this->componentMap[name] = pComponent;
//		printf("%s (%s::%d)\n", __func__, pComponent->getClassInfo().getName(), pComponent->getComponentId());
	}
	void registerAnAspect(unsigned name, unsigned localName, Component* pAspect) {
		this->aspectMap[pair<unsigned, unsigned>(name, localName)] = pAspect;
//		printf("%s (%s::%d)\n", __func__, pAspect->getClassInfo().getName(), pAspect->getComponentId());
	}
	void allocateAComponent(unsigned schedulerName, unsigned componentName) {
		this->allocationMap[componentName] = schedulerName;
		/*
		cout << _methodName()
			<< "("
			<< _componentName(this->schedulerMap[schedulerName])
			<< ","
			<< _componentName(this->componentMap[componentName])
			<< ")\n";
			*/
	}
	// <senderName, sender.receiverName>, receiverName
	void associateASenderNAReceiver(unsigned senderName, unsigned receiverId, unsigned receiverName) {
		this->sendersNReceiversMap[pair<unsigned, unsigned>(senderName, receiverId)] = receiverName;
		/*
		cout << _methodName()
			<< "("
			<< _componentName(this->componentMap[senderName])
			<< ","
			<< receiverId
			<< ","
			<< _componentName(this->componentMap[receiverName])
			<< ")\n";
			*/
	}

	// <sourceName, source.GroupName + vector<targetName>*
	void associateASourceNATarget(unsigned sourceName, unsigned groupId, unsigned targetName) {
		auto itr = this->sourcesNTargetsMap.find(pair<unsigned, unsigned>(sourceName, groupId));
		if (itr == this->sourcesNTargetsMap.end()) {
			this->sourcesNTargetsMap[pair<unsigned, unsigned>(sourceName, groupId)] = new vector<unsigned>();
		}
		this->sourcesNTargetsMap[pair<unsigned, unsigned>(sourceName, groupId)]->push_back(targetName);
	}

	/////////////////////////////////////////////////////////////////////////
	virtual void registerSystemSchedulers(Scheduler* pMain) = 0;
	virtual void registerSystemComponents() = 0;
	virtual void registerAspects() = 0;
	virtual void allocateSystemComponents() = 0;
	virtual void associateSystemSendersNReceivers() = 0;
	virtual void associateAspects() = 0;
	virtual void associateSystemSourcesNTargets() = 0;
	//////////////////////////////////////////////////////////////////////////
	virtual void registerUserShedulers() = 0;
	virtual void registerUserComponents() = 0;
	virtual void allocateUserComponents() = 0;
	virtual void associateUserSendersNReceivers() = 0;
	virtual void associateUserSourcesNTargets() = 0;

private:
	/////////////////////////////////////////////////////////////////////////
	// event mapping functions
	/////////////////////////////////////////////////////////////////////////
	// start system bootstrapping
	void initializeAsALifecycleManager(Event* pEvent) {
		this->SendAEvent(this->getUId(), (unsigned)EEventType::eRegisterSchedulers, 0, pEvent->getPArg());
	}
	void initializeAsALifecycleManagerReply(Event* pEvent) {
//		cout << _methodName() << _endl;
	}
	void finalizeAsALifecycleManager(Event* pEvent) {}

	// scheduler initialization
	void registerSchedulers(Event* pEvent) {
		Scheduler* pMain = (Scheduler*)pEvent->getPArg();
		// schedulers registreation
		this->registerSystemSchedulers(pMain);
		this->registerUserShedulers();

		// initialize Schedulers
		for (auto itr : this->schedulerMap) {
			this->SendAEvent(
				itr.second->getUId(),
				(unsigned)Scheduler::EEventType::eInitializeAsAScheduler
			);
		}
		this->countSchedulers = this->schedulerMap.size();
		if (this->countSchedulers == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eRegisterComponents);
		}

		/////////////////////////////////////////////////////////////////////
		this->createTasks();
		this->startTasks();
		/////////////////////////////////////////////////////////////////////

	}
	void registerSchedulersReply(Event* pEvent) {
		this->countSchedulers--;
		if (this->countSchedulers == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eRegisterComponents);
		}
	}

	// component registeration
	void registerComponents(Event* pEvent) {
		this->registerSystemComponents();
		this->registerUserComponents();
		this->registerAspects();

		// move to next
		this->SendAEvent(this->getUId(), (unsigned)EEventType::eAllocateComponents);
	}
	// component allocation
	void allocateComponents(Event* pEvent) {
		this->allocateSystemComponents();
		this->allocateUserComponents();

		for (auto itr : this->allocationMap) {
			UId targetSchedulerId = this->schedulerMap[itr.second]->getUId();
			Component* pAllocatedComponent = this->componentMap[itr.first];
			this->SendAEvent(targetSchedulerId, (unsigned)Scheduler:: EEventType::eAllocateAComponent, 0, pAllocatedComponent);
		}
		//////////////////////////////////////////////////////////////////////////////////////
		// aspects allocation
		for (auto itr : this->aspectMap) {
			UId targetSchedulerId = this->schedulerMap[(unsigned)EReceivers::eMain]->getUId();
			this->SendAEvent(targetSchedulerId, (unsigned)Scheduler::EEventType::eAllocateAComponent, 0, itr.second);
		}
		//////////////////////////////////////////////////////////////////////////////////////

		this->countAllocatedComponents = this->allocationMap.size() + this->aspectMap.size();
		if (this->countAllocatedComponents == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eAssociateSendersNReceivers);
		}
	}
	void allocateComponentsReply(Event* pEvent) {
		this->countAllocatedComponents--;
		if (this->countAllocatedComponents == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eAssociateSendersNReceivers);
		}
	}
	// associate senders and recievers
	void associateSendersNReceivers(Event* pEvent) {
		this->associateSystemSendersNReceivers();
		this->associateUserSendersNReceivers();
		this->associateAspects();

		// <senderName, sender.receiverName>, receiverName
		for (auto itrSendersNReceivers : this->sendersNReceiversMap) {
			Component* pSender = this->componentMap[itrSendersNReceivers.first.first];
			Component* pReceiver = this->componentMap[itrSendersNReceivers.second];

			this->SendAEvent(pSender->getUId(),
				(unsigned)Component::EEventType::eAssociateAReceiver, 
				itrSendersNReceivers.first.second,
				(ValueObject*) new (UId::spMemoryManager) UId(pReceiver->getUId()));
		}

		////////////////////////////////////////////////////////////////////////////
		// aspects associations
		for (auto itrAspect: this->aspectMap) {
			for (auto itrComponent : this->componentMap) {
				this->SendAEvent(
					itrComponent.second->getUId(),
					(unsigned)Component::EEventType::eAssociateAReceiver,
					itrAspect.first.second,
					&itrAspect.second->getUId()
				);
			}
		}
		////////////////////////////////////////////////////////////////////////////
		this->countSendersNReceivers = this->sendersNReceiversMap.size() + (this->componentMap.size() * this->aspectMap.size());
		if (this->countSendersNReceivers == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eAssociateSourcesNTargets);
		}
	}
	void associateSendersNReceiversReply(Event* pEvent) {
		this->countSendersNReceivers--;
		if (this->countSendersNReceivers == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eAssociateSourcesNTargets);
		}
	}
	// associate a source and targets
	void associateSourcesNTargets(Event* pEvent) {
		this->associateSystemSourcesNTargets();
		this->associateUserSourcesNTargets();

		// sourceName, source.groupName + vector<targetName>*
		for (auto itrSource : this->sourcesNTargetsMap) {
			Component *pSourceComponent = this->componentMap[itrSource.first.first];

			vector<UId>* pTargetUIdVector = new vector<UId>();
			for (auto itrTargetName : *itrSource.second) {
				Component* pTargetComponent = this->componentMap[itrTargetName];
				pTargetUIdVector->push_back(pTargetComponent->getUId());
			}
 			this->SendAEvent(pSourceComponent->getUId(),
				(unsigned)Component::EEventType::eAssociateTargets,
				itrSource.first.second,
				(ValueObject*)pTargetUIdVector);
		}
		this->countSourcesNTargets = this->sourcesNTargetsMap.size();
		if (this->countSourcesNTargets == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eInitializeComponents);
		}
	}
	void associateSourcesNTargetsReply(Event* pEvent) {
		this->countSourcesNTargets--;
		if (this->countSourcesNTargets == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eInitializeComponents);
		}
	}
	// component initialization
	void initializeComponents(Event* pEvent) {
		for (auto itr : this->componentMap) {
			this->SendAEvent(itr.second->getUId(),(unsigned)Component::EEventType::eInitialize);
		}
		this->countComponents = this->componentMap.size();
		if (this->countComponents == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eInitializeAsALifecycleManagerReply);
		}
	}
	void initializeComponentsReply(Event* pEvent) {
		this->countComponents--;
		if (this->countComponents == 0) {
			this->SendAEvent(this->getUId(), (unsigned)EEventType::eInitializeAsALifecycleManagerReply);
		}
	}

protected:
	// <componentName, ComponentPtr>
	map<unsigned, Component*> componentMap;
	size_t countComponents;

	// <<aspect Name, local Name in Component>, ComponentPtr>, receiverName
	map<pair<unsigned, unsigned>, Component*> aspectMap;
	size_t countAspects;

	// <schedulerName, SchedulerPtr>
	map<unsigned, Scheduler*> schedulerMap;
	size_t countSchedulers;

	// <schedulerName, componentName>
	map<unsigned, unsigned> allocationMap;
	size_t countAllocatedComponents;

	// <senderName, local name in sender>, receiverName
	map<pair<unsigned, unsigned>, unsigned> sendersNReceiversMap;
	size_t countSendersNReceivers;

	// <sourceName, local groupName in source>, vector<targetName>*
	map<pair<unsigned, unsigned>, vector<unsigned>*> sourcesNTargetsMap;
	size_t countSourcesNTargets;
};