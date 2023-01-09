#include "LifecycleManager.h"

#include "../../2Platform/Component/Component.h"
#include "../../2Platform/Scheduler/Scheduler.h"
#include "../../2Platform/Main/IMain.h"
#include "../../1Base/Aspect/Directory.h"


void LifecycleManager::RegisterEventTypes() {
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStopSystem] = "eStopSystem";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eInitializeAsALifecycleManager] = "eInitializeAsALifecycleManager";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager] = "eFinalizeAsALifecycleManager";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eRegisterSchedulers] = "eRegisterSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eInitializeSchedulers] = "eInitializeSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStartSchedulers] = "eStartSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eFinalizeSchedulers] = "eFinalizeSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStopSchedulers] = "eStopSchedulers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eRegisterComponents] = "eRegisterComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eAllocateComponents] = "eAllocateComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eAssociateSendersNReceivers] = "eAssociateSendersNReceivers";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eAssociateSourcesNTargets] = "eAssociateSourcesNTargets";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eInitializeComponents] = "eInitializeComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eFinalizeComponents] = "eFinalizeComponents";
	Directory::s_dirEvents[(unsigned)ILifecycleManager::EEventType::eStartComponents] = "eStartComponents";
}

void  LifecycleManager::RegisterExceptions() {
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eEventTypeError] = "eEventTypeError";
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eSchedulerNotRegistered] = "eSchedulerNotRegistered";
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eComponentNotRegistered] = "eComponentNotRegistered";
	Directory::s_dirExceptions[(unsigned)ILifecycleManager::EException::eInitializationReplyError] = "eInitializationReplyError";
}

void LifecycleManager::InitializeVariables() {
	LOG_HEADER(this->GetClassName(), __func__);
	this->m_mapSchedulers.Clear();
	this->m_mapComponents.Clear();
	this->m_mapAllocations.Clear();
	this->m_mapSendersNReceivers.Clear();
	this->m_mapSourcesNTargets.Clear();

	this->m_pMainScheduler = nullptr;
	LOG_FOOTER(this->GetClassName(), __func__);
}

void LifecycleManager::DeleteComponents() {
	LOG_HEADER(this->GetClassName(), __func__);
	for (auto itr : m_mapComponents) {
		if (itr.second != this && itr.second != this->m_pMainScheduler) {
			delete itr.second;
			Directory::s_dirComponents.Remove((long long)itr.second);
		}
	}
	for (auto itr : this->m_mapSourcesNTargets) {
		delete itr.second;
	}
	LOG_FOOTER(this->GetClassName(), __func__);
}

Scheduler* LifecycleManager::FindAScheduler(UId uId) {
	for (auto itr : this->m_mapSchedulers) {
		if (itr.second->GetUId() == uId) {
			return itr.second;
		}
	}
	return nullptr;
}
Component* LifecycleManager::FindAComponent(UId uId) {
	for (auto itr : this->m_mapComponents) {
		if (itr.second->GetUId() == uId) {
			return itr.second;
		}
	}
	return nullptr;
}

LifecycleManager::LifecycleManager(int nClassId, const char* pcClassName)
	: Component(nClassId, pcClassName)
	, m_pMainScheduler(nullptr)
{
	LOG(this->GetClassName(), __func__);
	this->RegisterEventTypes();
	this->RegisterExceptions();
}

LifecycleManager::~LifecycleManager() {
	LOG(this->GetClassName(), __func__);
}

void LifecycleManager::Initialize() {
	Component::Initialize();
}
void LifecycleManager::Finalize() {
	Component::Finalize();
}

void LifecycleManager::RegisterAScheduler(int name, Scheduler* pScheduler) {
	this->m_mapSchedulers[name] = pScheduler;
	LOG(__func__, Directory::s_dirComponents[pScheduler->GetComponentId()]);
	this->RegisterAComponent(name, pScheduler);
}

void LifecycleManager::RegisterAComponent(int name, Component* pComponent) {
	this->m_mapComponents[name] = pComponent;
	LOG(__func__, Directory::s_dirComponents[pComponent->GetComponentId()]);
}

void LifecycleManager::AllocateAComponent(int componentName, int schedulerName) {
	this->m_mapAllocations[componentName] = schedulerName;
	/*
	LOG(__func__,
		Directory::s_dirComponents[this->m_mapComponents[componentName]->GetComponentId()],
		Directory::s_dirComponents[this->m_mapSchedulers[schedulerName]->GetComponentId()]
	);
	*/
}

void LifecycleManager::AssociateASenderNAReceiver(int senderName, int receiverId, int receiverName) {
	this->m_mapSendersNReceivers[MapPairSender(senderName, receiverId)] = receiverName;
	Component* pSenderComponent = this->m_mapComponents[senderName];
	Component* pReceiverComponent = this->m_mapComponents[receiverName];
	/*
	LOG(__func__,
		Directory::s_dirComponents[pSenderComponent->GetComponentId()],
		Directory::s_dirComponents[pReceiverComponent->GetComponentId()]
	);
	*/
}

// <sourceName, source.GroupName + vector<tarGetName>*
void LifecycleManager::AssociateASourceNATarget(int nSourceName, int nGroupId, int nTarGetName) {
	auto itr = this->m_mapSourcesNTargets.Find(MapPairSource(nSourceName, nGroupId));
	if (itr == this->m_mapSourcesNTargets.end()) {
		this->m_mapSourcesNTargets[MapPairSource(nSourceName, nGroupId)] = new("VectorTargetNames") VectorTargetNames();
	}
	this->m_mapSourcesNTargets[MapPairSource(nSourceName, nGroupId)]->Add(nTarGetName);

	Component* pSourceComponent = this->m_mapComponents[nSourceName];
	Component* pTargetComponent = this->m_mapComponents[nTarGetName];
	/*
	LOG(__func__,
		Directory::s_dirComponents[pSourceComponent->GetComponentId()],
		nGroupId,
		Directory::s_dirComponents[pTargetComponent->GetComponentId()]
	);
	*/
}

/////////////////////////////////////////////////////////////////////////
void LifecycleManager::RegisterSystemSchedulers() {
	this->RegisterAScheduler((int)ILifecycleManager::EReceivers::eMainScheduler, m_pMainScheduler);
}
/////////////////////////////////////////////////////////////////////////
void LifecycleManager::RegisterSystemComponents() {
	this->RegisterAComponent((int)ILifecycleManager::EReceivers::eLifecycleManager, this);
}
/////////////////////////////////////////////////////////////////////////
void LifecycleManager::AllocateSystemComponents() {
	this->AllocateAComponent((int)ILifecycleManager::EReceivers::eLifecycleManager, (int)ILifecycleManager::EReceivers::eMainScheduler);
}
/////////////////////////////////////////////////////////////////////////
void LifecycleManager::AssociateSystemSendersNReceivers() {
	// Components -> LifecycleManager
	for (auto itr : this->m_mapComponents) {
		this->AssociateASenderNAReceiver(
			itr.first, (int)Component::EReceivers::eLifecycleManager, (int)ILifecycleManager::EReceivers::eLifecycleManager);
	}
	// lifecycleManager -> components
	for (auto itr : this->m_mapComponents) {
		this->AssociateASenderNAReceiver(
			(int)ILifecycleManager::EReceivers::eLifecycleManager, itr.first, itr.first);
	}
}
//////////////////////////////////////////////////////////////////////////
void LifecycleManager::AssociateSystemSourcesNTargets() {
}
//////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////
// InitializeAsALifecycleManager
///////////////////////////////////////////////////

// Register Schedulers
void LifecycleManager::RegisterSchedulers(Event* pEvent) {
	LOG_HEADER(__func__);
	this->RegisterSystemSchedulers();
	this->RegisterUserShedulers();
	LOG_FOOTER(__func__);
}

// Initialize Schedulers
void LifecycleManager::InitializeSchedulers(Event* pEvent) {
	LOG_HEADER(__func__);
	for (auto itr : m_mapSchedulers) {
		itr.second->InitializeAsAScheduler(0);
	}
	LOG_FOOTER(__func__);
}

// Start Schedulers
void LifecycleManager::StartSchedulers(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		for (auto itr : m_mapSchedulers) {
			itr.second->Start();
			this->SendReplyEvent(itr.second->GetUId(), (int)IScheduler::EEventType::eIsStarted);
		}
	}
}

// Register Components
void LifecycleManager::RegisterComponents(Event* pEvent) {
	LOG_HEADER(__func__);
	this->RegisterSystemComponents();
	this->RegisterUserComponents();
	LOG_FOOTER(__func__);
}

// Allocate Components
void LifecycleManager::AllocateComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		this->AllocateSystemComponents();
		this->AllocateUserComponents();

		for (auto& itrMapAllocation : this->m_mapAllocations) {
			// find target scheduler
			Component* pTargetScheduler = this->m_mapSchedulers[itrMapAllocation.second];
			if (pTargetScheduler == nullptr) {
				throw Exception((int)ILifecycleManager::EException::eSchedulerNotRegistered, this->GetClassName(), __func__, "eSchedulerNotRegistered");
			}
			// find component to be allocated
			Component* pComponent = this->m_mapComponents[itrMapAllocation.first];
			if (pComponent == nullptr) {
				throw Exception((int)ILifecycleManager::EException::eComponentNotRegistered, this->GetClassName(), __func__, "eComponentNotRegistered");
			}

			UId uIdTargetScheduler = pTargetScheduler->GetUId();
			Scheduler::ParamAllocateAComponent* pParamAllocateAComponent =
				new("ParamAllocateAComponent") Scheduler::ParamAllocateAComponent(pComponent);
			this->SendReplyEvent(
				uIdTargetScheduler
				, (int)IScheduler::EEventType::eAllocateAComponent
				, UNDEFINED
				, pParamAllocateAComponent
			);
		}
	}
}

// associate senders and recievers
void LifecycleManager::AssociateSendersNReceivers(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		this->AssociateSystemSendersNReceivers();
		this->AssociateUserSendersNReceivers();

		// <<senderName, sender.receiverName>, receiverName>
		for (auto& itrSendersNReceivers : this->m_mapSendersNReceivers) {
			Component* pSender = this->m_mapComponents[itrSendersNReceivers.first.first];
			Component* pReceiver = this->m_mapComponents[itrSendersNReceivers.second];

			Component::ParamAssociateAReceiver* pParamAssociateAReceiver
				= new("ParamAssociateAReceiver") Component::ParamAssociateAReceiver(itrSendersNReceivers.first.second, pReceiver->GetUId());
			this->SendReplyEvent(
				pSender->GetUId(),
				(int)Component::EEventType::eAssociateAReceiver,
				0,
				pParamAssociateAReceiver);
		}

	}
}

// associate a source and targets
void LifecycleManager::AssociateSourcesNTargets(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		this->AssociateSystemSourcesNTargets();
		this->AssociateUserSourcesNTargets();

		// sourceName, source.groupName + vector<tarGetName>*
		for (auto itrSource : this->m_mapSourcesNTargets) {
			Component* pSourceComponent = this->m_mapComponents[itrSource.first.first];
			Component::ParamAssociateATarget* pParamAssociateATarget
				= new("ParamAssociateATarget") Component::ParamAssociateATarget(itrSource.first.second);
			for (auto itrTargetName : *itrSource.second) {
				Component* pTargetComponent = this->m_mapComponents[itrTargetName];
				pParamAssociateATarget->GetVUIdTargetComponents().Add(pTargetComponent->GetUId());
			}
			this->SendReplyEvent(
				pSourceComponent->GetUId(),
				(int)Component::EEventType::eAssociateATarget,
				itrSource.first.second,
				pParamAssociateATarget);
		}
	}
}

// Initialize Components
void LifecycleManager::InitializeComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		for (auto& itrMapComponents : this->m_mapComponents) {
			this->SendReplyEvent(itrMapComponents.second->GetUId(), (int)Component::EEventType::eInitialize);
		}
	}
}

// start Components
void LifecycleManager::StartComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER("LifecycleManager", __func__);
		}
	}
	else {
		LOG_HEADER("LifecycleManager", __func__);
		this->StartComponents();
	}
}

//  InitializeAsALifecycleManager
void LifecycleManager::InitializeAsALifecycleManager(Event* pEvent) {
	if (pEvent->IsReply()) {
		switch (pEvent->GetReplyType()) {
		case (int)ILifecycleManager::EEventType::eRegisterSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eInitializeSchedulers);
			break;
		case (int)ILifecycleManager::EEventType::eInitializeSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eStartSchedulers);
			break;
		case (int)ILifecycleManager::EEventType::eStartSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eRegisterComponents);
			break;
		case (int)ILifecycleManager::EEventType::eRegisterComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eAllocateComponents);
			break;
		case (int)ILifecycleManager::EEventType::eAllocateComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eAssociateSendersNReceivers);
			break;
		case (int)ILifecycleManager::EEventType::eAssociateSendersNReceivers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eAssociateSourcesNTargets);
			break;
		case (int)ILifecycleManager::EEventType::eAssociateSourcesNTargets:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eInitializeComponents);
			break;
		case (int)ILifecycleManager::EEventType::eInitializeComponents:
			LOG_FOOTER(this->GetClassName(), __func__);
			break;
		default:
			throw Exception((int)ILifecycleManager::EException::eInitializationReplyError, this->GetClassName(), __func__);
			break;
		}
	}
	else {
		LOG_HEADER(this->GetClassName(), __func__);
		this->InitializeVariables();

		// unmarshalling
		ParamInitializeAsALifecycleManager* pParamInitializeAsALifecycleManager = (ParamInitializeAsALifecycleManager*)pEvent->GetPArg();
		this->m_pMainScheduler = pParamInitializeAsALifecycleManager->GetPMainScheduler();

		this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eRegisterSchedulers);

	}
}

///////////////////////////////////////////////////
// Event Mapping Methods before Unmarshalling
///////////////////////////////////////////////////

// Stop System
void LifecycleManager::StopSystem(Event* pEvent) {
	LOG_HEADER(__func__);
	this->SendNoReplyEvent(
		(int)ILifecycleManager::EReceivers::eMainScheduler,
		(int)IMain::EEventType::eFinalizeAsAMain);
	LOG_FOOTER(__func__);
}

///////////////////////////////////////////////////
// FinalizeAsALifecycleManager
///////////////////////////////////////////////////

void LifecycleManager::FinalizeComponents(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		for (auto& itr : this->m_mapComponents) {
			this->SendReplyEvent(itr.second->GetUId(), (int)Component::EEventType::eFinalize);
		}
	}
}

void LifecycleManager::StopSchedulers(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->IsAllReplied()) {
			for (auto itr : m_mapSchedulers) {
				itr.second->Join();
			}
			LOG_FOOTER(__func__);
		}
	}
	else {
		LOG_HEADER(__func__);
		for (auto itr : m_mapSchedulers) {
			this->SendReplyEvent(itr.second->GetUId(), (int)IScheduler::EEventType::eStop);
		}
	}
}

void LifecycleManager::FinalizeSchedulers(Event* pEvent) {
	LOG_HEADER(__func__);
	for (auto itr : m_mapSchedulers) {
		itr.second->FinalizeAsAScheduler();
	}
	LOG_FOOTER(__func__);
}

void LifecycleManager::FinalizeAsALifecycleManager(Event* pEvent) {
	if (pEvent->IsReply()) {
		switch (pEvent->GetReplyType()) {
		case (int)ILifecycleManager::EEventType::eFinalizeComponents:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eStopSchedulers);
			break;
		case (int)ILifecycleManager::EEventType::eStopSchedulers:
			this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eFinalizeSchedulers);
			break;
		case (int)ILifecycleManager::EEventType::eFinalizeSchedulers:
			this->DeleteComponents();
			LOG_FOOTER(this->GetClassName(), __func__);
			break;
		default:
			throw Exception((int)ILifecycleManager::EException::eInitializationReplyError, this->GetClassName(), __func__);
			break;
		}
	}
	else {
		LOG_HEADER(this->GetClassName(), __func__);
		this->SendReplyEvent(this->GetUId(), (int)ILifecycleManager::EEventType::eFinalizeComponents);

	}
}

void LifecycleManager::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (int)ILifecycleManager::EEventType::eInitializeAsALifecycleManager:
		this->InitializeAsALifecycleManager(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eRegisterSchedulers:
		this->RegisterSchedulers(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eInitializeSchedulers:
		this->InitializeSchedulers(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eStartSchedulers:
		this->StartSchedulers(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eRegisterComponents:
		this->RegisterComponents(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eAllocateComponents:
		this->AllocateComponents(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eAssociateSendersNReceivers:
		this->AssociateSendersNReceivers(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eAssociateSourcesNTargets:
		this->AssociateSourcesNTargets(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eInitializeComponents:
		this->InitializeComponents(pEvent);
		break;

	case (int)ILifecycleManager::EEventType::eStartComponents:
		this->StartComponents(pEvent);
		break;

	case (int)ILifecycleManager::EEventType::eStopSystem:
		this->StopSystem(pEvent);
		break;

	case (int)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager:
		this->FinalizeAsALifecycleManager(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eFinalizeComponents:
		this->FinalizeComponents(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eStopSchedulers:
		this->StopSchedulers(pEvent);
		break;
	case (int)ILifecycleManager::EEventType::eFinalizeSchedulers:
		this->FinalizeSchedulers(pEvent);
		break;
	default:
		Component::ProcessAEvent(pEvent);
		break;
	}
}
