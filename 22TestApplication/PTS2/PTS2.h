#pragma once

#include <22TestApplication/PTS2/Config.h>
#define _PTS2_Id _GET_TESTLAYER_UID(_ELayer_PTestSuit::_ePTS2)
#define _PTS2_Name "PTS2"

#include <21TestPlatform/TestCase/TestSuite.h>

#include <22TestApplication/PTS2/PTC21/PTC21.h>
#include <22TestApplication/PTS2/PTC22/PTC22.h>

class PTS2: public TestSuite {
public:
	PTS2(
		unsigned typeId = _PTS2_Id,
		const char* pClassName = _PTS2_Name)
		: TestSuite(typeId, pClassName)
	{
		this->add(new("PTC21") PTC21());
		this->add(new("PTC22") PTC22());
	}
};

