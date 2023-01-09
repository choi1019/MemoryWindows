#include "PLifecycleManager.h"

#include "../../../Platform/5PPlatform/Pscheduler/PScheduler.h"
#include "../../../Platform/6PTechnical/PTimer/PTimer.h"
#include "../../../Platform/6PTechnical/PSocketServer/PSocketServer.h"
#include "../../Domain/Sender/Sender.h"
#include "../../Domain/Receiver/Receiver.h"

#include "../../../Platform/6PTechnical/PSocketClient/PSocketClient.h"

PLifecycleManager::PLifecycleManager(
		unsigned typeId,
		const char* pClassName)
		: LifecycleManager(typeId, pClassName)
	{}
PLifecycleManager::~PLifecycleManager() {}

void PLifecycleManager::RegisterUserShedulers() {
//	this->RegisterAScheduler((int)EComponents::eScheduler1, new("eScheduler1") PScheduler());
//	this->RegisterAScheduler((int)EComponents::eScheduler2, new("eScheduler2") PScheduler());
}
void PLifecycleManager::RegisterUserComponents() {
	this->RegisterAComponent((int)EComponents::eSocketClient, new("PSocketClient") PSocketClient());
//	this->RegisterAComponent((int)EComponents::eSender, new("eSender") Sender());
//	this->RegisterAComponent((int)EComponents::eReceiver, new("eReceiver") Receiver());
//	this->RegisterAComponent((int)EComponents::eTimer, new("eTimer") PTimer(0, 200));
//	this->RegisterAComponent((int)EComponents::eSocketServer, new("eSocketServer") PSocketServer());
}
void PLifecycleManager::AllocateUserComponents() {
	this->AllocateAComponent((int)EComponents::eSocketClient, (int)ILifecycleManager::EReceivers::eMainScheduler);
//	this->AllocateAComponent((int)EComponents::eSender, (int)EComponents::eScheduler1);
//	this->AllocateAComponent((int)EComponents::eTimer, (int)EComponents::eScheduler1);
//	this->AllocateAComponent((int)EComponents::eReceiver, (int)EComponents::eScheduler2);
//	this->AllocateAComponent((int)EComponents::eSocketServer, (int)EComponents::eScheduler2);
}
void PLifecycleManager::AssociateUserSendersNReceivers() {
//	this->AssociateASenderNAReceiver(
//		(int)EComponents::eSender, (int)ISender::EReceivers::eReceiver, (int)EComponents::eReceiver);
//	this->AssociateASenderNAReceiver(
//		(int)EComponents::eSender, (int)ISender::ESources::eTimer, (int)EComponents::eTimer);
}
void PLifecycleManager::AssociateUserSourcesNTargets() {
//	this->AssociateASourceNATarget((int)EComponents::eTimer, (int)Timer::EGroups::eGroup1, (int)EComponents::eSender);
}

void PLifecycleManager::Initialize() {
	LifecycleManager::Initialize();
}
void PLifecycleManager::Finalize() {
	LifecycleManager::Finalize();
}

void PLifecycleManager::StartComponents() {
	this->SendReplyEvent((int)EComponents::eSocketClient, (int)Component::EEventType::eRun);
//	this->SendReplyEvent((int)EComponents::eSocketServer, (int)Component::EEventType::eStart);
//	this->SendReplyEvent((int)EComponents::eSender, (int)Component::EEventType::eStart);
}

void PLifecycleManager::StopComponents() {
//	this->SendReplyEvent((int)EComponents::eSocketServer, (int)Component::EEventType::eStop);
}
