#include "PTimer.h"

PTimer::PTimer(
	int nDueTime, int nInterval,
	int nClassId, const char* pcClassName)
	: PComponent(nClassId, pcClassName)
	, m_hTimer(NULL)
	, m_nDueTime(nDueTime)
	, m_nInterval(nInterval)
{
	m_hTimer = CreateWaitableTimer(
		NULL,                   // Default security attributes
		FALSE,                  // Create auto-reset timer
		TEXT("PTimer"));		// Name of waitable timer
	if (m_hTimer == NULL) {
		throw Exception((int)Timer::EError::eTimerCreationError, this->GetClassName(), __func__, "CreateWaitableTimer");
	}

	this->RegisterEventTypes();
	this->RegisterExceptions();
}
PTimer::~PTimer() {
	CloseHandle(m_hTimer);
}

void PTimer::RegisterEventTypes() {
	PComponent::RegisterEventTypes();
	Directory::s_dirEvents[(unsigned)Timer::EEventType::eTimeOut] = "eTimeOut";
}
void PTimer::RegisterExceptions() {
	PComponent::RegisterExceptions();
}

void PTimer::Initialize() {
	PComponent::Initialize();
}
void PTimer::Finalize() {
	PComponent::Finalize();
}

void PTimer::TimeOut() {
	LOG("PTimer::eTimeOut");
	this->BeginSequence(nullptr);
	this->SendTargetEvents((int)EGroups::eGroup1, (int)Timer::EEventType::eTimeOut);
	this->EndSequence();
}

void PTimer::Run() {
	PComponent::Run();
	LOG("PTimer::Run");

	while (this->GetEState() == Component::EState::eRunning) {
		if (WaitForSingleObject(m_hTimer, INFINITE) != WAIT_OBJECT_0)
			printf("WaitForSingleObject failed (%d)\n", GetLastError());
		else printf("Timer was signaled.\n");

		this->TimeOut();
	}
}

void PTimer::Start() {
	PComponent::Start();
	LOG("PTimer::Start");

	LARGE_INTEGER   liDueTime;
	__int64         qwDueTime;

	// Create an integer that will be used to signal the timer 
	// nDueTime seconds from now.
	qwDueTime = -(__int64)this->m_nDueTime * _SECOND;

	// Copy the relative time into a LARGE_INTEGER.
	liDueTime.LowPart = (DWORD)(qwDueTime & 0xFFFFFFFF);
	liDueTime.HighPart = (LONG)(qwDueTime >> 32);

	BOOL bSuccess = SetWaitableTimer(
		m_hTimer,	// Handle to the timer object
		&liDueTime,		// When timer will become signaled
		m_nInterval,	// Periodic timer interval in milliseconds
		nullptr,		// Completion routine
		nullptr,		// Argument to the completion routine
		FALSE);			// Do not restore a suspended system
	if (!bSuccess) {
		throw Exception((int)Timer::EError::eSetTimerError, this->GetClassName(), __func__, "SetWaitableTimer");
	}
}

void PTimer::Stop() {
	PComponent::Stop();
	LOG("PTimer::Stop");

	BOOL bSuccess = CancelWaitableTimer(m_hTimer);
	if (!bSuccess) {
		throw Exception((int)Timer::EError::eSetTimerError, this->GetClassName(), __func__, "SetWaitableTimer");
	}
}