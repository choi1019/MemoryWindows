#pragma once

#include "../typedef.h"
#define _TestMain_Id _GET_CLASS_UID(_ELayer_TestBase::_eTestMain)
#define _TestMain_Name "TestMain"


#include "TestSuite.h"

class TestMain: public TestSuite {
public:
	TestMain(int nClassId = _TestMain_Id, const char* pClassName = _TestMain_Name)
		: TestSuite(nClassId, pClassName)
	{
	}
	virtual ~TestMain() {
	}

	virtual void Initialize() {
		TestSuite::Initialize();
	}
	virtual void Finalize() {
		TestSuite::Finalize();
	}
};