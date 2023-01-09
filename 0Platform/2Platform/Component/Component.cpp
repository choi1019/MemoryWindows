#include "Component.h"
#include "../../2Platform/EventQueue/EventQueue.h"

//unsigned Component::s_uCounter = 0;

void Component::RegisterEventTypes() {
	Directory::s_dirEvents[(unsigned)EEventType::eAssociateAReceiver] = "eAssociateAReceiver";
	Directory::s_dirEvents[(unsigned)EEventType::eAssociateATarget] = "eAssociateATarget";
	Directory::s_dirEvents[(unsigned)EEventType::eInitialize] = "eInitialize";
	Directory::s_dirEvents[(unsigned)EEventType::eFinalize] = "eFinalize";
	Directory::s_dirEvents[(unsigned)EEventType::eStart] = "eStart";
	Directory::s_dirEvents[(unsigned)EEventType::eStop] = "eStop";
	Directory::s_dirEvents[(unsigned)EEventType::eRun] = "eRun";
	Directory::s_dirEvents[(unsigned)EEventType::ePause] = "ePause";
}

void Component::RegisterExceptions() {
	ComponentPart::RegisterExceptions();
}

Component::Component(unsigned uClassId, const char* pcClassName)
	: ComponentPart(uClassId, pcClassName)
{
	this->SetPUId(new("UId") UId(this->GetObjectId(), nullptr));
	//		BaseObject::s_pMemory->SetId(this);

	this->SetPMReceivers(new("ReceiversMap") Map<unsigned, UId, MAXRECEIVERCOMPONENTS>());
	this->SetPMTargetsGroups(new("TargetsMap") Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>());

	this->m_eState = EState::eCreated;

	// component id directory
	Directory::s_dirComponents[this->GetComponentId()] = Directory::s_dirObjects[(long long)this];
	this->RegisterEventTypes();
	this->RegisterExceptions();
	// LOG
	LOG("new", Directory::s_dirComponents[this->GetComponentId()]);
}
Component::~Component() {
	LOG("delete", Directory::s_dirComponents[this->GetComponentId()]);

	delete this->GetPUId();
	delete this->GetPMReceivers();
	// delete target group vectors
	for (auto itr : *this->GetPMTargetsGroups()) {
		delete itr.second;
	}
	delete this->GetPMTargetsGroups();
	// LOG
}

void Component::AddPart(unsigned uName, ComponentPart* pComponentPart) {
	pComponentPart->SetPUId(this->GetPUId());
	pComponentPart->SetPMReceivers(this->GetPMReceivers());
	pComponentPart->SetPMTargetsGroups(this->GetPMTargetsGroups());

	this->m_mComponentParts.Add(uName, pComponentPart);
}
ComponentPart* Component::GetPart(unsigned uName) {
	return this->m_mComponentParts[uName];
}

void Component::SetPEventQueue(EventQueue* pEventQueue) {
	this->GetPUId()->SetPEventQueue(pEventQueue);
	this->m_eState = EState::eAllocated;
	LOG(this->GetClassName(), __func__, Directory::s_dirComponents[pEventQueue->GetSchedulerId()]);
}

void Component::AssociateAReceiver(unsigned receiverName, UId receiverUId) {
	this->GetPMReceivers()->Add(receiverName, receiverUId);
	LOG(
		Directory::s_dirComponents[this->GetComponentId()]
		, __func__
		, Directory::s_dirComponents[receiverUId.GetComponentId()]);
}

void Component::AssociateATarget(unsigned uGroupName, Vector<UId>& vNewUIdTargetComponents) {
	// find a groupName
	auto itrTargetGroup = this->GetPMTargetsGroups()->Find(uGroupName);
	if (itrTargetGroup == this->GetPMTargetsGroups()->end()) {
		// if not exist, create a new one
		itrTargetGroup->first = uGroupName;
		itrTargetGroup->second = new("Vector") Vector<UId, MAXTARGETCOMPONENTS>();
		this->GetPMTargetsGroups()->Add(*itrTargetGroup);
	}
	for (auto itrNewUIdTargetComponent : vNewUIdTargetComponents) {
		bool bExist = false;
		for (UId uIdTargetComponent : *(itrTargetGroup->second)) {
			if (uIdTargetComponent == itrNewUIdTargetComponent) {
				// arleady exist
				bExist = true;
				break;
			}
		}
		if (!bExist) {
			// if newTargetUId is new one, then add
			itrTargetGroup->second->Add(itrNewUIdTargetComponent);
			LOG(Directory::s_dirComponents[this->GetComponentId()], __func__
				, uGroupName
				, Directory::s_dirComponents[itrNewUIdTargetComponent.GetComponentId()]);
		}
	}
}

void Component::Initialize() {
	ComponentPart::Initialize();
	this->m_eState = EState::eInitialized;
	LOG(Directory::s_dirComponents[this->GetComponentId()], __func__);
}
void Component::Finalize() {
	ComponentPart::Finalize();
	this->m_eState = EState::eFinalized;
	LOG(Directory::s_dirComponents[this->GetComponentId()], __func__);
}

void Component::Start() {
	this->m_eState = EState::eStarted;
}
void Component::Stop() {
	this->m_eState = EState::eStopped;
}
void Component::Run() {
	this->m_eState = EState::eRunning;
}
void Component::Pause() {
	this->m_eState = EState::ePaused;
}

/////////////////////////////////////////////////////////////
// event mapping functions
/////////////////////////////////////////////////////////////
void Component::AssociateAReceiver(Event* pEvent) {
	if (this->m_eState < EState::eAllocated) {
		throw Exception((unsigned)EException::eNotAllocated, this->GetClassName(), __func__, "eNotAllocated");
	}

	ParamAssociateAReceiver* pParamAssociateAReceiver = (ParamAssociateAReceiver*)pEvent->GetPArg();
	unsigned uReceiverName = pParamAssociateAReceiver->GetReceiverName();
	UId uIdReceiverComponent = pParamAssociateAReceiver->GetUIdReceiverComponent();
	this->AssociateAReceiver(uReceiverName, uIdReceiverComponent);

	this->m_eState = EState::eAssociated;
	//		Log(this->GetClassName(), "AssociateAReceiver", uReceiverName).Println();
}

void Component::AssociateATarget(Event* pEvent) {
	if (this->m_eState < EState::eAllocated) {
		throw Exception((unsigned)EException::eNotAllocated, this->GetClassName(), __func__, "eNotAllocated");
	}

	ParamAssociateATarget* pParamAssociateATarget = (ParamAssociateATarget*)pEvent->GetPArg();
	unsigned uGroupName = pParamAssociateATarget->GetGroupName();
	Vector<UId> vUIdTargetComponent = pParamAssociateATarget->GetVUIdTargetComponents();
	this->AssociateATarget(uGroupName, vUIdTargetComponent);

	//		this->SendAReplyEvent(pEvent, 0, nullptr);
	this->m_eState = EState::eTargeted;
}

void Component::Initialize(Event* pEvent) {
	this->Initialize();
}
void Component::Finalize(Event* pEvent) {
	this->Finalize();
}

void Component::Start(Event* pEvent) {
	this->Start();
}

void Component::Stop(Event* pEvent) {
	this->Stop();
}

void Component::Run(Event* pEvent) {
	this->Run();
}
void Component::Pause(Event* pEvent) {
	this->Pause();
}

void Component::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (unsigned)EEventType::eAssociateAReceiver:
		this->AssociateAReceiver(pEvent);
		break;
	case (unsigned)EEventType::eAssociateATarget:
		this->AssociateATarget(pEvent);
		break;
	case (unsigned)EEventType::eInitialize:
		this->Initialize(pEvent);
		break;
	case (unsigned)EEventType::eStart:
		this->Start(pEvent);
		break;
	case (unsigned)EEventType::eStop:
		this->Stop(pEvent);
		break;
	case (unsigned)EEventType::eRun:
		this->Run(pEvent);
		break;
	case (unsigned)EEventType::ePause:
		this->Pause(pEvent);
		break;
	case (unsigned)EEventType::eFinalize:
		this->Finalize(pEvent);
		break;
	default:
		throw Exception(
			(unsigned)EException::eEventNotSupported,
			this->GetClassName(), __func__,
			String("eEventNotSupported:") + (unsigned)pEvent->GetType());
		break;
	}
}