#include "PComponent.h"

#include <Windows.h>
#undef GetClassName

DWORD WINAPI _CompnentProc(LPVOID lpParam)
{
	PComponent* pPComponent = (PComponent*)lpParam;
	pPComponent->Run();
	return 1;
}

PComponent::PComponent(
	int nClassId, const char* pcClassName)
	: Component(nClassId, pcClassName)
	, m_hThread(nullptr)
	, m_dwThreadId(UNDEFINED)
{
	m_hThread = CreateThread(
		NULL,				// default security attributes
		0,					// default stack size
		(LPTHREAD_START_ROUTINE)_CompnentProc,
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
PComponent::~PComponent() {
	CloseHandle(this->m_hThread);
}

void PComponent::RegisterEventTypes() {
	Component::RegisterEventTypes();
}
void PComponent::RegisterExceptions() {
	Component::RegisterExceptions();
}

void PComponent::Initialize() {
	Component::Initialize();
}
void PComponent::Finalize() {
	Component::Finalize();
}

void PComponent::Start() {
	LOG("PCompnent::Start");
	Component::Start();
	ResumeThread(m_hThread);
}

void PComponent::Pause() {
	LOG("PCompnent::Pause");
	Component::Pause();
}

void PComponent::Stop() {
	Component::Stop();
	LOG("PCompnent::Stop");
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

void PComponent::Run() {
	Component::Run();
	LOG("PCompnent::Run");
}