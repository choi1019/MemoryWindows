#pragma once

#include "../typedef.h"
#define _Parser_Id _GET_CLASS_UID(_ELayer_Domain::_eParser)
#define _Parser_Name "Parser"

#include "../../../0Platform/2Platform/Component/ComponentPart.h"
#include "../../../0Platform/1Base/Aspect/Log.h"

class Parser : public ComponentPart {

public:
	Parser(
		int nClassId = _Parser_Id,
		const char* pcClassName = _Parser_Name)
		: ComponentPart(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~Parser() {
	}

	virtual void Parse(int nAtt1, String sAtt2) {
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
