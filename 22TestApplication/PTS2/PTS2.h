#pragma once

#include "Config.h"
#define _PTS2_ID _GET_TESTSUIT_UID(_ELayer_PTestSuit::_ePTS2)
#define _PTS2_NAME "PTS2"

#include "../../21TestPlatform/TestCase/TestSuite.h"
#include "PTC21/PTC21.h"
#include "PTC22/PTC22.h"

class PTS2: public TestSuite {
public:
	PTS2(
		unsigned typeId = _PTS2_ID,
		const char* pClassName = _PTS2_NAME)
		: TestSuite(typeId, pClassName)
	{
		this->add(new("PTC21") PTC21());
		this->add(new("PTC22") PTC22());
	}
};

