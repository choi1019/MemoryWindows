#include "Main.h"

Main::Main(
	unsigned uClassId,
	const char* pcClassName)
	: Scheduler(uClassId, pcClassName)
	, m_pLifecycleManager(nullptr)
{
}
Main::~Main() {}

void Main::Run() {
	Scheduler::Run();
}
// do nothing as a scheduler
void Main::InitializeAsAScheduler(int uPriority) {}

void Main::Start() {}
void Main::Join() {}

void Main::Pause() {}
void Main::Resume() {}
void Main::Stop() {}

void Main::FinalizeAsAScheduler() {}

void Main::SendStartEvent() {
	// initialize as a main, scheduler is not running
	this->BeginSequence(nullptr);
	this->SendNoReplyEvent(
		this->GetUId(),
		(unsigned)IMain::EEventType::eInitializeAsAMain,
		0,
		nullptr   // root event
	);
	this->EndSequence();
}

void Main::InitializeAsAMain(Event* pEvent) {
	if (pEvent->IsReply()) {
		LOG_FOOTER(this->GetClassName(), __func__, "finished");
		this->SendNoReplyEvent(
			this->m_pLifecycleManager->GetUId(), 
			(int)ILifecycleManager::EEventType::eStartComponents
		);
	}
	else {
		LOG_HEADER(this->GetClassName(), __func__, "started");

		// send a trigger event
		ILifecycleManager::ParamInitializeAsALifecycleManager* pParamParamInitializeAsALifecycleManager
			= new("ParamInitializeAsALifecycleManager") ILifecycleManager::ParamInitializeAsALifecycleManager(this);
		this->SendReplyEvent(
			this->m_pLifecycleManager->GetUId()
			, (unsigned)ILifecycleManager::EEventType::eInitializeAsALifecycleManager
			, UNDEFINED
			, pParamParamInitializeAsALifecycleManager
		);
	}

}
void Main::FinalizeAsAMain(Event* pEvent) {
	if (pEvent->IsReply()) {
		Scheduler::Stop();
		LOG_FOOTER(__func__);
	}
	else {
		LOG_HEADER(__func__);
		this->SendReplyEvent(this->m_pLifecycleManager->GetUId(),
			(unsigned)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager);
	}
}

void Main::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (unsigned)IMain::EEventType::eInitializeAsAMain:
		this->InitializeAsAMain(pEvent);
		break;
	case (unsigned)IMain::EEventType::eFinalizeAsAMain:
		this->FinalizeAsAMain(pEvent);
		break;
	default:
		Scheduler::ProcessAEvent(pEvent);
		break;
	}
}

