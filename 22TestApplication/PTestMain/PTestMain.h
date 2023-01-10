#pragma once

#include "../typedef.h"
#define _PTESTMAIN_ID _GET_CLASS_UID(_ELayer_TestApplication::_ePTestMain)
#define _PTESTMAIN_NAME "PTestMain"

#include "../../21TestPlatform/TestCase/TestMain.h"
#include "Config.h"
#include "../PTS1/PTS1.h"

class PTestMain : public TestMain {
private:
	PTS1* pPTS1;

public:
	PTestMain(
		int nClassId = _PTESTMAIN_ID,
		const char* pcClassName = _PTESTMAIN_NAME)
		: TestMain(nClassId, pcClassName)
		, pPTS1(nullptr)
	{
	}
	virtual ~PTestMain() {
	}

	virtual void Initialize() {
		TestMain::Initialize();

		this->pPTS1 = new PTS1();
		this->add(this->pPTS1);
	}
	virtual void Finalize() {
		delete this->pPTS1;

		TestMain::Finalize();
	}
};
