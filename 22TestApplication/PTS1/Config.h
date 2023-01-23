#pragma once
#include "../PTestMain/Config.h"

enum class _ELayer_PTS1 {
	_eBegin = _GET_TESTCASE_UID(_ELayer_PTestSuit::_ePTS1),
	_ePTC11, 
	_ePTC12,
	_ePTC13,
	_ePTC14,
	_eEnd
};

#define SIZE_SYSTEM_MEMORY 2048
#define SIZE_USER_MEMORY 6144
//#define SIZE_USER_MEMORY 4098
#define SIZE_PAGE 1024
#define SIZE_SLOT_UNIT 32


