#pragma once

#include <22TestApplication/PTestMain/typedef.h>
#define _PTestMain_ID _GET_TESTCLASS_UID(_ETestComponent_PMain::_ePTestMain)
#define _PTestMain_NAME "PTestMain"

#include <21TestPlatform/TestCase/TestMain.h>

class PTestMain : public TestMain {
public:
	PTestMain(unsigned nClassId = _PTestMain_ID,	const char* pcClassName = _PTestMain_NAME);
	~PTestMain();
	void Initialize() override;
	void Finalize() override;
};
