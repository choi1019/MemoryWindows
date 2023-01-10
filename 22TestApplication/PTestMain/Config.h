#pragma once

#define _GET_TESTSUIT_UID(ID)		(int)ID*1000
#define _GET_TESTCASE_UID(ID)		(int)ID*1000

enum class _ELayer_PTestSuit {
	_eBegin,
	_ePTestMain,
	_ePTS1,
	_ePTS2,
	_eEnd

};
