#pragma once

#include "../../../13Component/Component/Component.h"
#include "ISender.h"
#include "IReceiver.h"

#include "Receiver.h"
#include "../../../13Component/Scheduler/Scheduler.h"


class Sender : public Component, public ISender {

public:
	Sender(
		unsigned typeId = _PTC41_SENDER_ID_,
		const char* pClassName = _PTC41_SENDER_NAME_)
		: Component(typeId, pClassName)
	{}
	virtual ~Sender() {}
	void Initialize() {
		Component::Initialize();
	}
	void Finalize() {
		Component::Finalize();
	}

	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (int)ISender::EEventType::eStart:
			Send(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}

private:
	void Send(Event* pEvent) {
		if (pEvent->IsReply()) {
			IReceiver::ResultTest* pReultTest = reinterpret_cast<IReceiver::ResultTest* > (pEvent->GetPArg());
			SendReply(pReultTest->GetResult());
		}
		else {
			IReceiver::ParamTest* pParamTest = new("PTC32_Receiver::TestParam") IReceiver::ParamTest();
			this->SendReplyEvent(
				(unsigned)ISender::EReceivers::eReceiver
				, (unsigned)IReceiver::EEventType::eTest
				, pEvent->GetlArg()
				, pParamTest
			);
		}
	}

protected:
	virtual void SendReply(int result) {
		Log(this->GetClassStrName(), __func__, String("Result:") + result).Println();
	}
};
