#pragma once

#include "Config.h"
#define _PTS1_ID _GET_TESTSUIT_UID(_ELayer_PTestSuit::_ePTS1)
#define _PTS1_NAME "PTS1"

#include "../../21TestPlatform/TestCase/TestSuite.h"

#include "Config.h"
#include "PTC11/PTC11.h"

class PTS1: public TestSuite {
public:
	PTS1(
		unsigned typeId = _PTS1_ID,
		const char* pClassName = _PTS1_NAME)
		: TestSuite(typeId, pClassName)
	{
		this->add(new PTC11());
	}
};

