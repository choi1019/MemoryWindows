#pragma once

#define UNDEFINED -1

#define _GET_TESTLAYER_UID(ID)	(unsigned)ID*100
#define _GET_TESTCASE_UID(ID)	(unsigned)ID*100

enum class _ETestLayer {
	_eBegin = 20,

	_eTestPlatform = 21,
	_eTestApplication = 22,

	_eEnd,
};

enum class _ELayer_TestPlatform {
	_eBegin = _GET_TESTLAYER_UID(_ETestLayer::_eTestPlatform),

	_eTestRoot,
	_eTestObject,

	_eTestMemoryManager,
	_eTestException,
	_eTestLog,

	_eTestCase,
	_eTestSuite,
	_eTestMain,
	_eTestApplication,

};

// for printf
#include <stdio.h>
// for malloc
#include <stdlib.h>
// for string
#include <string>
using namespace std;

