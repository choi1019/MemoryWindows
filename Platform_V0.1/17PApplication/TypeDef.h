//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PAPPLICATION_TYPEDEF
#define PAPPLICATION_TYPEDEF "PAPPLICATION_TYPEDEF"
#include "../../01Global/TypeDef.h"

enum EPApplication {
	_ePApplicationBegin_ = GETLAYERID(__ePApplication__),
	_ePMainTask_,
	_ePLifecycleManager_,
	_ePApplicationEnd_
};

#endif
