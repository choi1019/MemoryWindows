#pragma once

#include "../typedef.h"
#define _Generator_Id _GET_CLASS_UID(_ELayer_Domain::_eGenerator)
#define _Generator_Name "Generator"

#include "../../../0Platform/2Platform/Component/ComponentPart.h"
#include "../../../0Platform/1Base/Aspect/Log.h"
#undef GetClassName

class Generator : public ComponentPart {

public:
	Generator(
		int nClassId = _Generator_Id,
		const char* pcClassName = _Generator_Name)
		: ComponentPart(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~Generator() {
	}

	virtual void Generate(int nAtt1, String sAtt2) {
		LOG(this->GetClassName(), __func__, "");
	}

protected:
	virtual void RegisterExceptions() {
	}
	void Initialize() {
		ComponentPart::Initialize();
	}
	void Finalize() {
		ComponentPart::Finalize();
	}
};