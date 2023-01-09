#pragma once

#include "IReceiver.h"
#include "../../../Platform/2Platform/Component/Component.h"
#include "../../../Platform/1Base/Aspect/Log.h"

class Receiver : public Component, public IReceiver {

public:
	Receiver(
		int nClassId = _Receiver_Id,
		const char* pcClassName = _Receiver_Name)
		: Component(nClassId, pcClassName)
	{
		this->RegisterEventTypes();
		this->RegisterExceptions();
	}
	virtual ~Receiver() {
	}

protected:
	virtual void RegisterEventTypes() {
		Component::RegisterEventTypes();
		Directory::s_dirEvents[(unsigned)IReceiver::EEventType::eCompute] = "eCompute";
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

	virtual Result* Compute(int nAtt1, String sAtt2) {
		LOG(this->GetClassName(), __func__, "");
		Result* pResult = new("ResultProcess") Result(nAtt1 + 1, sAtt2 + "Result");
		return pResult;
	}

private:
	void Compute(Event* pEvent) {
		ParamCompute* pParamCompute = reinterpret_cast<ParamCompute*>(pEvent->GetPArg());
		Result* pResult = this->Compute(pParamCompute->GetAtt1(), pParamCompute->GetAtt2());
		// prepare return result
		delete pEvent->GetPArg();
		pEvent->SetlArg(pEvent->GetlArg() + (long)1);
		pEvent->SetPArg(pResult);
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IReceiver::EEventType::eCompute:
			this->Compute(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
