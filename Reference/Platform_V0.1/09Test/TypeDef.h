//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef TEST_TYPEDEF
#define TEST_TYPEDEF "TEST"
#include "../../01Global/TypeDef.h"

enum ETest {
	_eTestBegin_ = GETLAYERID(__eTest__),
	_eTestSuit_,
	_eTestCase_,
	_eTestEnd_
};

#endif
