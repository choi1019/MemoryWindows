#pragma once

#include "../typedef.h"
#define _PTestMainId _GET_CLASS_UID(_ELayer_TestApplication::_ePTestMain)
#define _PTestMain_Name "PTestMain"

#include "../../11TestPlatform/TestCase/TestMain.h"
#include "Config.h"
#include "../PTS1/PTS1_Main.h"

class PTestMain : public TestMain {
private:
	PTS1_Main* pPTS1_Main;

public:
	PTestMain(
		int nClassId = _PTestMainId,
		const char* pcClassName = _PTestMain_Name)
		: TestMain(nClassId, pcClassName)
		, pPTS1_Main(nullptr)
	{
	}
	virtual ~PTestMain() {
	}

	virtual void Initialize() {
		TestMain::Initialize();

		this->pPTS1_Main = new PTS1_Main();
		this->add(this->pPTS1_Main);
	}
	virtual void Finalize() {
		delete this->pPTS1_Main;

		TestMain::Finalize();
	}
};
