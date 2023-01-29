#ifndef _ETESTLAYER_PLATFORM
#define _ETESTLAYER_PLATFORM

#define UNDEFINED -1

#define _GET_TLAYER_UID(ID)		(int)ID*1000
#define _GET_TCLASS_UID(ID)		(int)ID*1000

enum class _ETestLayer {
	_eBegin = 20,

	_eTestLayer_Platform = 21,
	_eTestApplication = 22,

	_eEnd,
};

enum class _ELayer_TestPlatform {
	_eBegin = _GET_TLAYER_UID(_ETestLayer::_eTestLayer_Platform),

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

#endif