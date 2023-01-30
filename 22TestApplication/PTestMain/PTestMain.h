#pragma once

#include <22TestApplication/PTestMain/Config.h>
#define _PTestMain_ID _GET_TESTCASE_UID(_ELayer_PTestSuit::_ePTestMain)
#define _PTestMain_NAME "PTestMain"

#include <21TestPlatform/TestCase/TestMain.h>

class PTestMain : public TestMain {
public:
	PTestMain(
		int nClassId = _PTestMain_ID,
		const char* pcClassName = _PTestMain_NAME);
	~PTestMain();
	void Initialize();
	void Finalize();
};
