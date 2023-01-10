#pragma once

#include "../typedef.h"
#define _PTS1_ID _GET_CLASS_UID(_ELayer_TestApplication::_ePTS1)
#define _PTS1_NAME "PTS1"

#include "../../11TestPlatform/TestCase/TestSuite.h"

#include "Config.h"
#include "../PTS1/PTC11/PTC11.h"

class PTS1: public TestSuite {
private:
	PTC11* pPTC11;

public:
	PTS1(
		unsigned typeId = _PTS1_ID,
		const char* pClassName = _PTS1_NAME)
		: TestSuite(typeId, pClassName)
		, pPTC11(nullptr)
	{
	}
	virtual ~PTS1() {

	}
	void Initialize() {
		TestSuite::Initialize();

		this->pPTC11 = new PTC11();
		this->add(this->pPTC11);
	}

	void Finalize() {
		delete this->pPTC11;

		TestSuite::Finalize();
	}
};

