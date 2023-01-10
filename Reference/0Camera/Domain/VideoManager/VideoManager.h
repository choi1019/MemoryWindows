#pragma once

#include "../../../0Platform/2Platform/Component/Component.h"

// provided interfacec
#include "IVideoManager.h"

// required interface
#include "../../../0Platform/3Technical/Timer/Timer.h"
#include "../VideoInput/IVideoInput.h"
#include "../VideoOutput/IVideoOutput.h"

class VideoManager : public IVideoManager, public Component {

private:
	int m_nCount;

public:
	VideoManager(
		int nClassId = _VideoManager_Id,
		const char* pcClassName = _VideoManager_Name)
		: Component(nClassId, pcClassName)
		, m_nCount(0)
	{}
	virtual ~VideoManager() {}

protected:
	virtual void RegisterEventTypes() {
		Component::RegisterEventTypes();
	}

	virtual void RegisterExceptions() {
		Component::RegisterExceptions();
	}

	void Initialize() {
		Component::Initialize();
	}
	void Finalize() {
		Component::Finalize();
	}

	void Stop() {
		Component::Stop();

	}

private:
	void Start(Event* pEvent) {
		if (pEvent->IsReply()) {

		}
		else {
			Component::Start(pEvent);
			this->SendReplyEvent((int)IVideoManager::EReceivers::eVideoOutput, (int)Component::EEventType::eStart);
//			this->SendReplyEvent((int)IVideoManager::ESources::eTimer, (int)Component::EEventType::eStart);
		}
	}
	void Stop(Event* pEvent) {
		if (pEvent->IsReply()) {
		}
		else {
			Component::Stop(pEvent);
			this->SendReplyEvent((int)IVideoManager::ESources::eTimer, (int)Component::EEventType::eStop);
		}
	}

	void Run(Event* pEvent) {
		if (pEvent->IsReply()) {
			if (pEvent->GetReplyType() == (int)IVideoInput::EEventType::eParseFrame) {
				this->SendReplyEvent((int)IVideoManager::EReceivers::eVideoOutput, (int)IVideoOutput::EEventType::eGenerateFrame);
			}
			else if (pEvent->GetReplyType() == (int)IVideoOutput::EEventType::eGenerateFrame) {
				LOG_FOOTER("ProcessAFrame");
			}
		}
		else {
			LOG_HEADER("ProcessAFrame");
			this->SendReplyEvent((int)IVideoManager::EReceivers::eVideoInput, (int)IVideoInput::EEventType::eParseFrame);
		}
	}

	void Pause(Event* pEvent) {
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (int)Timer::EEventType::eTimeOut:
			this->Run(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
