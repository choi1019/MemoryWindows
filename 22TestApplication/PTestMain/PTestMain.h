#pragma once

#include <22TestApplication/PTestMain/Config.h>
#define _PTestMain_ID _GET_TCLASS_UID(_ELayer_PTestSuit::_ePTestMain)
#define _PTestMain_NAME "PTestMain"

#include <21TestPlatform/TestCase/TestMain.h>
#include <22TestApplication/PTS1/PTS1.h>
#include <22TestApplication/PTS2/PTS2.h>

class PTestMain : public TestMain {
public:
	PTestMain(
		int nClassId = _PTestMain_ID,
		const char* pcClassName = _PTestMain_NAME)
		: TestMain(nClassId, pcClassName)
	{
	}
	~PTestMain() {
	}


	void Initialize() {
		// <int SIZE_SYSTEM_MEMORY, int SIZE_USER_MEMORY, int SIZE_PAGE, int SIZE_SLOT_UNIT>
		this->add(new("PTS1-1") PTS1<4096, 6144, 1024, 32>());
		this->add(new("PTS1-2") PTS1<4096, 6144, 256, 8>());
		this->add(new("PTS1-3") PTS1<4096, 8192, 2048, 128>());
//		this->add(new("PTS2-1") PTS2());
	}

	void Finalize() {
	}
};
