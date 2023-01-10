#pragma once

#include "IReceiver.h"
#include "../../../17Aspect/Log/Log.h"
#include "../../../13Component/Scheduler/Scheduler.h"

class Receiver : public Component, public IReceiver {

public:
	Receiver(
		unsigned typeId = _PTC41_RECEIVER_ID_,
		const char* pClassName = _PTC41_RECEIVER_NAME_)
		: Component(typeId, pClassName)
	{
	}
	virtual ~Receiver() {
	}

	void Initialize() {
		Component::Initialize();
	}
	void Finalize() {
		Component::Finalize();
	}

	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IReceiver::EEventType::eTest:
			this->Test(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
private:
	void Test(Event* pEvent) {
		ParamTest* pParamTest = reinterpret_cast<ParamTest*>(pEvent->GetPArg());
		ResultTest* pResultTest = this->Test(pParamTest);
		// prepare return result
		pEvent->SetlArg(pEvent->GetlArg() + (long)1);
		pEvent->SetPArg(pResultTest);
	}

protected:
	virtual ResultTest* Test(ParamTest* pTParamTest) {
		Log(this->GetClassStrName(), __func__, "").Println();
		ResultTest* pResultTest = new("PTC41_Receiver::TestResult") ResultTest();
		return pResultTest;
	}
};
