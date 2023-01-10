#pragma once

#include "../../../Platform/2Platform/Component/Component.h"

// provided interfacec
#include "ISender.h"
// required interface
#include "../Receiver/IReceiver.h"
#include "../../../Platform/3Technical/Timer/Timer.h"
#include "../../../Platform/2Platform/LifecycleManager/ILifecycleManager.h"

class Sender : public ISender, public Component {

private:
	int m_nCount;

public:
	Sender(
		int nClassId = _Sender_Id,
		const char* pcClassName = _Sender_Name)
		: Component(nClassId, pcClassName)
		, m_nCount(0)
	{}
	virtual ~Sender() {}

protected:
	virtual void RegisterEventTypes() {
		Component::RegisterEventTypes();
//		Directory::s_dirEvents[(unsigned)ISender::EEventType::eReceiver] = "eTimeOut";
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

private:
	virtual void Start(Event *pEvent) {
		if (pEvent->IsReply()) {
			LOG_FOOTER(Directory::s_dirComponents[this->GetComponentId()], __func__, "Timer_Reply(Started)");
		}
		else {
			LOG_HEADER(Directory::s_dirComponents[this->GetComponentId()], __func__);
			this->SendReplyEvent((int)ISender::ESources::eTimer, (int)IComponent::EEventType::eStart);
			m_nCount = 0;
		}
	}

	virtual void Stop(Event* pEvent) {
		LOG("PTimer::Stop");
	}

	virtual void Compute(IReceiver::Result* pResult) {
		LOG(Directory::s_dirComponents[this->GetComponentId()], __func__);
	}

	void Compute(Event* pEvent) {
		if (pEvent->IsReply()) {
			if (pEvent->GetReplyType() == (int)IReceiver::EEventType::eCompute) {
				IReceiver::Result* pResult = (IReceiver::Result * )pEvent->GetPArg();
				this->Compute(pResult);
			}
			else if (pEvent->GetReplyType() == (int)Component::EEventType::eStop) {
				Event::s_pMemory->Show("Sender::Compute");
				LOG(Directory::s_dirComponents[this->GetComponentId()], "Reply", "Timer::eStopTimer");

				this->SendNoReplyEventLast(
					(int)Component::EReceivers::eLifecycleManager,
					(int)ILifecycleManager::EEventType::eStopSystem);

			}
			else {

			}
		}
		else {
			m_nCount++;
			if (m_nCount < 10) {
				IReceiver::ParamCompute* pParamCompute = new("ParamCompute") IReceiver::ParamCompute(1, "Process");
				this->SendReplyEvent(
					(int)ISender::EReceivers::eReceiver
					, (int)IReceiver::EEventType::eCompute
					, m_nCount
					, pParamCompute
				);
			}
			else if (m_nCount == 10) {
				this->SendReplyEvent((int)ISender::ESources::eTimer, (int)Component::EEventType::eStop);
			}
			else {
				LOG(Directory::s_dirComponents[this->GetComponentId()], "TimerOut", "Overflow", m_nCount);
			}
		}
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (int)Timer::EEventType::eTimeOut:
			Compute(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
