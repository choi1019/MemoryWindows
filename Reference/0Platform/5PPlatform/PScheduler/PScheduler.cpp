#include "PScheduler.h"

DWORD WINAPI _ThreadProc(LPVOID lpParam)
{
	PScheduler* pPScheduler = (PScheduler*)lpParam;
	pPScheduler->Run();
	return 1;
}

PScheduler::PScheduler(
	int uClassId,
	const char* pcClassName)
	: Scheduler(uClassId, pcClassName)
	, m_hThread(nullptr)
	, m_dwThreadId(UNDEFINED)
{
}
PScheduler::~PScheduler() 
{
}

void PScheduler::InitializeAsAScheduler(int uPriority) {
	Scheduler::InitializeAsAScheduler(uPriority);

	PEventQueue* pPEventQueue = new("PEventQueue") PEventQueue(this->GetComponentId());
	this->SetPEventQueue(pPEventQueue);

	m_hThread = CreateThread(
		NULL,				// default security attributes
		0,					// default stack size
		(LPTHREAD_START_ROUTINE)_ThreadProc,
		this,				// no thread function arguments
		CREATE_SUSPENDED,	// default creation flags
		&m_dwThreadId		// receive thread identifier
	);

	if (m_hThread == nullptr) {
		throw Exception(
			(unsigned)IScheduler::EError::eThreadCreationFailed,
			this->GetClassName(),
			__func__,
			"eThreadCreationFailed"
		);
	}
}
void PScheduler::FinalizeAsAScheduler() {
	Scheduler::FinalizeAsAScheduler();

	CloseHandle(m_hThread);

	delete this->GetPEventQueue();
}

void PScheduler::Start() {
	ResumeThread(m_hThread);
}

// mutual exclusion /////////////////////////////////////////
void PScheduler::Join() {
	unsigned result = WaitForSingleObject(m_hThread, INFINITE);
	if (result != 0) {
		throw Exception(
			(unsigned)IScheduler::EError::eThreadTerminationFailed,
			this->GetClassName(),
			__func__,
			String("_eThreadTerminationFailed:") + result
		);
	}
}

void PScheduler::Initialize() {
	Scheduler::Initialize();
}
void PScheduler::Finalize() {
	Scheduler::Finalize();
}
