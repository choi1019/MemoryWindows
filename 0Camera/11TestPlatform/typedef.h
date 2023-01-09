#pragma once

#include "../../0Platform/1Base/typedef.h"


enum class _ELayer_TestBase {
	_eBegin = _GET_LAYER_UID(_ELayer::_eTestPlatform),

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


