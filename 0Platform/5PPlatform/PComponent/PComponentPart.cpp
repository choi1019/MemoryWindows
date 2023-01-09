#include "PComponentPart.h"

#undef GetClassName

DWORD WINAPI _CompnentPartProc(LPVOID lpParam)
{
	PComponentPart* pPComponentPart = (PComponentPart*)lpParam;
	pPComponentPart->Run();
	return 1;
}

PComponentPart::PComponentPart(unsigned uClassId, const char* acClassName)
	: ComponentPart(uClassId, acClassName)
{
	this->RegisterExceptions();

	m_hThread = CreateThread(
		NULL,				// default security attributes
		0,					// default stack size
		(LPTHREAD_START_ROUTINE)_CompnentPartProc,
		this,				// thread function arguments
		CREATE_SUSPENDED,	// default creation flags
		&m_dwThreadId		// receive thread identifier
	);

	if (m_hThread == nullptr) {
		/*
		throw Exception(
			(unsigned)IScheduler::EError::eThreadCreationFailed,
			this->GetClassName(),
			__func__,
			"eThreadCreationFailed"
		);
		*/
	}
}

PComponentPart::~PComponentPart() {
	CloseHandle(this->m_hThread);
}

void PComponentPart::RegisterExceptions() {
	ComponentPart::RegisterExceptions();
}

void PComponentPart::Initialize() {
	ComponentPart::Initialize();
}
void PComponentPart::Finalize() {
	ComponentPart::Finalize();
}

void PComponentPart::Start() {
	LOG("PComponentPart::Start");
	ResumeThread(m_hThread);
}

void PComponentPart::Pause() {
	LOG("PComponentPart::Pause");
}

void PComponentPart::Stop() {
	LOG("PComponentPart::Stop");
	unsigned result = WaitForSingleObject(m_hThread, INFINITE);
	if (result != 0) {
		/*
		throw Exception(
			(unsigned)IScheduler::EError::eThreadTerminationFailed,
			this->GetClassName(),
			__func__,
			String("_eThreadTerminationFailed:") + result
		);
		*/
	}
}

void PComponentPart::Run() {
	LOG("PComponentPart::Run");
}