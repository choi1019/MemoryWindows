#include "PLifecycleManager.h"

#include "../../../0Platform/5PPlatform/Pscheduler/PScheduler.h"
#include "../../../0Platform/6PTechnical/PTimer/PTimer.h"
#include "../../Domain/VideoManager/VideoManager.h"
#include "../../Domain/VideoInput/VideoInput.h"
#include "../../Domain/VideoOutput/VideoOutput.h"

PLifecycleManager::PLifecycleManager(
		unsigned typeId,
		const char* pClassName)
		: LifecycleManager(typeId, pClassName)
	{}
PLifecycleManager::~PLifecycleManager() {}

void PLifecycleManager::RegisterUserShedulers() {
	this->RegisterAScheduler((int)EComponents::eScheduler1, new("eScheduler1") PScheduler());
	this->RegisterAScheduler((int)EComponents::eScheduler2, new("eScheduler2") PScheduler());
}
void PLifecycleManager::RegisterUserComponents() {
	this->RegisterAComponent((int)EComponents::eTimer33, new("eTimer33") PTimer(0, 33));
	this->RegisterAComponent((int)EComponents::eVideoManager, new("VideoManager") VideoManager());
	this->RegisterAComponent((int)EComponents::eVideoInput, new("VideoInput") VideoInput());
	this->RegisterAComponent((int)EComponents::eVideoOutput, new("VideoOutput") VideoOutput());
}
void PLifecycleManager::AllocateUserComponents() {
	this->AllocateAComponent((int)EComponents::eTimer33, (int)EComponents::eScheduler1);
	this->AllocateAComponent((int)EComponents::eVideoManager, (int)EComponents::eScheduler1);
	this->AllocateAComponent((int)EComponents::eVideoInput, (int)EComponents::eScheduler2);
	this->AllocateAComponent((int)EComponents::eVideoOutput, (int)EComponents::eScheduler2);
}
void PLifecycleManager::AssociateUserSendersNReceivers() {
	this->AssociateASenderNAReceiver(
		(int)EComponents::eVideoManager, (int)IVideoManager::ESources::eTimer, (int)EComponents::eTimer33);
	this->AssociateASenderNAReceiver(
		(int)EComponents::eVideoManager, (int)IVideoManager::EReceivers::eVideoInput, (int)EComponents::eVideoInput);
	this->AssociateASenderNAReceiver(
		(int)EComponents::eVideoManager, (int)IVideoManager::EReceivers::eVideoOutput, (int)EComponents::eVideoOutput);
}
void PLifecycleManager::AssociateUserSourcesNTargets() {
	this->AssociateASourceNATarget((int)EComponents::eTimer33, (int)Timer::EGroups::eGroup1, (int)EComponents::eVideoManager);
}

void PLifecycleManager::Initialize() {
	LifecycleManager::Initialize();
}
void PLifecycleManager::Finalize() {
	LifecycleManager::Finalize();
}

void PLifecycleManager::StartComponents() {
	this->SendReplyEvent((int)EComponents::eVideoManager, (int)Component::EEventType::eStart);
}

void PLifecycleManager::StopComponents() {
	this->SendReplyEvent((int)EComponents::eVideoManager, (int)Component::EEventType::eStop);
}
