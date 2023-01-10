#include "PMain.h"

#include "../../PApplication/PLifecycleManager/PLifecycleManager.h"
#include "../../../Platform/5PPlatform/PEventQueue/PEventQueue.h"

#include "../../../Platform/1Base/Aspect/Directory.h"
#include "../../../Platform/1Base/Aspect/Log.h"

PMain::PMain(unsigned uClassId,
	const char* pcClassName)
	: Main(uClassId, pcClassName)
{
}
PMain::~PMain() {
}

void PMain::AddParts(MemoryManager* pMemoryManager) {
	this->AddPart(pMemoryManager->GetObjectId(), pMemoryManager);
	for (IMemory* pIMemory : pMemoryManager->GetVMemory()) {
		ComponentPart* pComponentPart = dynamic_cast<ComponentPart*>(pIMemory);
		this->AddPart(pComponentPart->GetObjectId(), pComponentPart);
	}
}
void PMain::BootstrapSystem() {
	LOG_HEADER(this->GetClassName(), __func__);

	// Main EventQueue
	PEventQueue* pPEventQueue = new("PMain::PEventQueue") PEventQueue(this->GetComponentId());
	LOG("new", Directory::s_dirObjects[(long long)pPEventQueue]);
	this->SetPEventQueue(pPEventQueue);
	LOG_NEXTLINE();

	// Lifecycle Manager
	this->SetPLifecycleManager(new("PLifecycleManager") PLifecycleManager());
	this->AllocateAComponent(this->GetPLifecycleManager());

	LOG_FOOTER(this->GetClassName(), __func__);

	this->SendStartEvent();
}

void PMain::CloseSystem() {
	delete this->GetPEventQueue();
	delete this->GetPLifecycleManager();
}


