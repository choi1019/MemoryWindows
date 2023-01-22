#pragma once

#include "Config.h"
#define _PTESTMAIN_ID _GET_CLASS_UID(_ELayer_PTestSuit::_ePTestMain)
#define _PTESTMAIN_NAME "PTestMain"

#include "../../21TestPlatform/TestCase/TestMain.h"
#include "Config.h"
#include "../PTS1/PTS1.h"
//#include "../PTS2/PTS2.h"

class PTestMain : public TestMain {
public:
	PTestMain(
		int nClassId = _PTESTMAIN_ID,
		const char* pcClassName = _PTESTMAIN_NAME)
		: TestMain(nClassId, pcClassName)
	{
		this->add(new PTS1());
//		this->add(new PTS2());
	}
	~PTestMain() {
		DeleteTestSuites();
	}
};
