#include "Scheduler.h"

#include "../../1Base/Aspect/Exception.h"

void Scheduler::InitializeVarialbes() {
	this->m_mComponents.Clear();
	//		this->m_pEventQueue->Clear();
}
void Scheduler::DeleteVarialbes() {
	LOG_HEADER(Directory::s_dirComponents[this->GetComponentId()], "EventQueue");
	this->m_pEventQueue->ShowState(Directory::s_dirComponents[this->GetComponentId()].c_str());

//	this->GetPEventQueue()->ShowState("");
	Event* pEvent = this->m_pEventQueue->Front();
	while (pEvent != nullptr) {
		LOG(__func__, Directory::s_dirEvents[pEvent->GetType()]);
		Event *pPoppedEvent = this->m_pEventQueue->Pop();
		if (pPoppedEvent != nullptr) {
			delete pEvent;
		}
		pEvent = this->m_pEventQueue->Front();
	}
	LOG_FOOTER(Directory::s_dirComponents[this->GetComponentId()], "EventQueue");
}

void Scheduler::RegisterEventTypes() {
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eInitializeAsAScheduler] = "eInitializeAsAScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eIsStarted] = "eIsSchedulerStarted";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eAllocateAComponent] = "eAllocateAComponent";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eStart] = "eStartScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::ePause] = "ePauseScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eResume] = "eResumeScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eDellocateAComponent] = "eDellocateAComponent";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eStop] = "eStopScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eFinalizeAsAScheduler] = "eFinalizeAsAScheduler";
}

Scheduler::Scheduler(
	unsigned classId, const char* className)
	: Component(classId, className)
	//		, m_nSchedulerId(s_uCounter++)
	, m_pEventQueue(nullptr)
{
	this->RegisterEventTypes();
	this->m_eState = IScheduler::EState::eCreated;
}
Scheduler::~Scheduler() {
}

void Scheduler::Initialize() {
	Component::Initialize();
}
void Scheduler::Finalize() {
	Component::Finalize();
}


//	int GetSchedulerId() { return this->m_nSchedulerId; }
EventQueue* Scheduler::GetPEventQueue() { 
	return this->m_pEventQueue; 
}

void Scheduler::SetPEventQueue(EventQueue* pEventQueue) {
	this->m_pEventQueue = pEventQueue;
	this->AllocateAComponent(this);
}

// jobs to do before the thread is started
void Scheduler::InitializeAsAScheduler(int uPriority) {
	this->InitializeVarialbes();
	this->m_eState = IScheduler::EState::eInitializedAsAScheduler;
}
void Scheduler::FinalizeAsAScheduler() {
	this->m_eState = IScheduler::EState::eFinalizedAsAScheduler;
	this->DeleteVarialbes();
}

void Scheduler::Run() {
	m_eState = IScheduler::EState::eStarted;
	LOG("Scheduler::StartSchedulers", Directory::s_dirComponents[this->GetComponentId()]);
	while (IScheduler::EState::eStopped != this->m_eState)
	{
		RunOnce();
	}
	LOG("Scheduler::StopSchedulers", Directory::s_dirComponents[this->GetComponentId()]);
}

void Scheduler::Pause() {
	m_eState = IScheduler::EState::ePaused;
}
void Scheduler::Resume() {
	m_eState = IScheduler::EState::eStarted;
}
void Scheduler::Stop() {
	m_eState = IScheduler::EState::eStopped;
}

void Scheduler::IsStarted(Event* pEvent) {
}
void Scheduler::Pause(Event* pEvent) {
	this->Pause();
}
void Scheduler::Resume(Event* pEvent) {
	this->Resume();
}
void Scheduler::Stop(Event* pEvent) {
	this->Stop();
}

void Scheduler::RunOnce()
{
	try {
		// get pEvent from the queue
		Event* pEvent = pEvent = m_pEventQueue->Pop();
		if (pEvent != nullptr) {
			// if the event is reply
			if (pEvent->IsReply()) {
				this->PrepareReplyEvent(pEvent);
			}
			// find the target component
			UId uidTarget = pEvent->GetUIdTarget();
			Component* pTargetComponent = m_mComponents[uidTarget.GetComponentId()];
			if (pTargetComponent == nullptr) {
				// delete pEvent;
				throw Exception((unsigned)IScheduler::EError::eComponentNotFound, this->GetClassName(), __func__, "eComponentNotFound");
			}
			// process a event
			pTargetComponent->BeginSequence(pEvent);
			try {
				pTargetComponent->ProcessAEvent(pEvent);
			}
			catch (Exception& exception) {
				exception.Println();
			}
			// post process pEvent
			if (pEvent->IsReply()) {
				delete pEvent;
			}
			else {
				if (pEvent->GetESyncType() == Event::ESyncType::eAsync) {
					delete pEvent;
				}
				else {
					pEvent->SetBReply(true);
					pEvent->GetUIdSource().GetPEventQueue()->PushFront(pEvent);
				}
			}

			pTargetComponent->EndSequence();
		}
	}
	catch (Exception& exception) {
		exception.Println();
	}
}

void Scheduler::AllocateAComponent(Component* pComponent) {
	pComponent->SetPEventQueue(this->GetPEventQueue());
	m_mComponents.Add(pComponent->GetComponentId(), pComponent);
	LOG(_Scheduler_Name, __func__
		, Directory::s_dirComponents[this->GetComponentId()]
		, Directory::s_dirComponents[pComponent->GetComponentId()]
	);
}
void Scheduler::DellocateAComponent(Component* pComponent) {
	m_mComponents.Remove(pComponent->GetComponentId());
}


/// Event Mapping Functions: Unmarshalling
void Scheduler::InitializeAsAScheduler(Event* pEvent) {
	unsigned uPriority = (unsigned)pEvent->GetlArg();
	this->InitializeAsAScheduler(uPriority);
}
void Scheduler::FinalizeAsAScheduler(Event* pEvent) {
	this->FinalizeAsAScheduler();
}

void Scheduler::AllocateAComponent(Event* pEvent) {
	ParamAllocateAComponent* pParamAllocateAComponent = (ParamAllocateAComponent*)pEvent->GetPArg();
	Component* pComponentAllocated = pParamAllocateAComponent->GetPComponentAllocated();
	if (pComponentAllocated == nullptr) {
		throw Exception((int)IScheduler::EError::eNullPoint, this->GetClassName(), __func__, "eNullPoint");
	}
	else {
		this->AllocateAComponent(pComponentAllocated);
	}
}
void Scheduler::DellocateAComponent(Event* pEvent) {
	ParamAllocateAComponent* pParamAllocateAComponent = (ParamAllocateAComponent*)pEvent->GetPArg();
	DellocateAComponent(pParamAllocateAComponent->GetPComponentAllocated());
}


void Scheduler::ProcessAEvent(Event* pEvent)
{
	switch (pEvent->GetType()) {
	case (unsigned)IScheduler::EEventType::eInitializeAsAScheduler:
		this->InitializeAsAScheduler(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eIsStarted:
		this->IsStarted(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eAllocateAComponent:
		this->AllocateAComponent(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eDellocateAComponent:
		this->DellocateAComponent(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::ePause:
		this->Pause(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eResume:
		this->Resume(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eStop:
		this->Stop(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eFinalizeAsAScheduler:
		this->FinalizeAsAScheduler(pEvent);
		break;
	default:
		Component::ProcessAEvent(pEvent);
		break;
	}
}