#pragma once

#include "IVideoInput.h"
#include "../../../0Platform/2Platform/Component/Component.h"
#include "Parser.h"
#include "../../../0Platform/1Base/Aspect/Log.h"

class VideoInput : public Component, public IVideoInput {

private:
	Parser* pParser;
public:
	VideoInput(
		int nClassId = _VideoInput_Id,
		const char* pcClassName = _VideoInput_Name)
		: Component(nClassId, pcClassName)
	{
		this->pParser = new("Parser") Parser();
		this->RegisterEventTypes();
		this->RegisterExceptions();
	}
	virtual ~VideoInput() {
	}

protected:
	virtual void RegisterEventTypes() {
		Component::RegisterEventTypes();
		Directory::s_dirEvents[(unsigned)IVideoInput::EEventType::eParseFrame] = "eParseFrame";
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
	void ParseFrame(Event* pEvent) {
		LOG(this->GetClassName(), __func__, "");
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoInput::EEventType::eParseFrame:
			this->ParseFrame(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
