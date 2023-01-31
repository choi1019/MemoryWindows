#pragma once

#include <typedefTest.h>

enum class _ELayer_TestPlatform {
	_eBegin = _GET_TESTLAYER_UID(_ETestLayer::_eTestPlatform),
	
	_eTestObject,

	_eTestMemoryManager,
	_eTestException,
	_eTestLog,

	_eTestCase,
	_eTestSuite,
	_eTestMain,
	_eTestApplication,

};