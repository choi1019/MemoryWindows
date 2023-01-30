#pragma once

#define UNDEFINED -1

#define _GET_TESTLAYER_UID(ID)	(unsigned)ID*10
#define _GET_TESTCOMPONENT_UID(ID)	(unsigned)ID*100
#define _GET_TESTCLASS_UID(ID)	(unsigned)ID*100

enum class _ETestLayer {
	_eBegin = 20,

	_eTestPlatform = 21,
	_eTestApplication = 22,

	_eEnd,
};

// for printf
#include <stdio.h>
// for malloc
#include <stdlib.h>
// for string
#include <string>
using namespace std;

