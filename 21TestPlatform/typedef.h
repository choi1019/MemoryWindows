#pragma once

#define UNDEFINED -1

#define _GET_LAYER_UID(ID)		(int)ID*1000
#define _GET_CLASS_UID(ID)		(int)ID*1000

enum class _ETestLayer {
	_eBegin = 20,

	_eTestPlatform = 21,
	_eTestApplication = 22,

	_eEnd,
};

enum class _ELayer_TestPlatform {
	_eBegin = _GET_LAYER_UID(_ETestLayer::_eTestPlatform),

	_eTestRoot,
	_eTestMemoryManager,
	_eTestException,
	_eTestLog,

	_eTestObject,
	_eTestCase,
	_eTestSuite,
	_eTestMain,
	_eTestApplication,

};

#include <stdlib.h>
#include <string>
using namespace std;


