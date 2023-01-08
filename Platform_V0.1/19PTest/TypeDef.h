//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PTEST_TYPEDEF
#define PTEST_TYPEDEF "PTEST"
#include "../../01Global/TypeDef.h"

enum EPTest {
	_ePTestBegin_ = GETLAYERID(__ePTest__),
	_ePTestSuit_,
	_ePTestCase_,
	_ePTestEnd_
};

#endif
