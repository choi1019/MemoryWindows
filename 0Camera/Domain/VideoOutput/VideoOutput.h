#pragma once

#include "IVideoOutput.h"
#include "../../../0Platform/6PTechnical/PSocketServer/PSocketServer.h"
#include "Generator.h"
#include "../../../0Platform/1Base/Aspect/Log.h"
#undef GetClassName

class VideoOutput : public PSocketServer, public IVideoOutput {

private:
	Generator* pGenerator;
public:
	VideoOutput(
		int nClassId = _VideoOutput_Id,
		const char* pcClassName = _VideoOutput_Name)
		: PSocketServer(nClassId, pcClassName)
	{
		this->pGenerator = new("Generator") Generator();
		this->RegisterEventTypes();
		this->RegisterExceptions();
	}
	virtual ~VideoOutput() {
	}

protected:
	virtual void RegisterEventTypes() {
		PSocketServer::RegisterEventTypes();
		Directory::s_dirEvents[(unsigned)IVideoOutput::EEventType::eGenerateFrame] = "eGenerate";
	}
	virtual void RegisterExceptions() {
		PSocketServer::RegisterExceptions();
	}

	void Initialize() {
		PSocketServer::Initialize();
	}
	void Finalize() {
		PSocketServer::Finalize();
	}

private:
	void Generate(Event* pEvent) {
		LOG(this->GetClassName(), __func__, "");
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoOutput::EEventType::eGenerateFrame:
			this->Generate(pEvent);
			break;
		default:
			PSocketServer::ProcessAEvent(pEvent);
			break;
		}
	}
};
