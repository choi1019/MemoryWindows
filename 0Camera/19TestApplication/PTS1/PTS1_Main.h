#pragma once

#include "../typedef.h"
#define _PTS1_Main_Id _GET_CLASS_UID(_ELayer_TestApplication::_ePTS1_Main)
#define _PTS1_Main_Name "PTS1_Main"

#include "../../11TestPlatform/TestCase/TestSuite.h"

#include "Config.h"
#include "../PTS1/PTC11/PTC11_Main.h"

class PTS1_Main: public TestSuite {
private:
	PTC11_Main* pPTC11_Main;

public:
	PTS1_Main(
		unsigned typeId = _PTS1_Main_Id,
		const char* pClassName = _PTS1_Main_Name)
		: TestSuite(typeId, pClassName)
		, pPTC11_Main(nullptr)
	{
	}
	virtual ~PTS1_Main() {

	}
	void Initialize() {
		TestSuite::Initialize();

		this->pPTC11_Main = new PTC11_Main();
		this->add(this->pPTC11_Main);
	}

	void Finalize() {
		delete this->pPTC11_Main;

		TestSuite::Finalize();
	}
};

