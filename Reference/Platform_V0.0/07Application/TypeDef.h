//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef APPLICATION_TYPEDEF
#define APPLICATION_TYPEDEF "APPLICATION_TYPEDEF"
#include "../../01Global/TypeDef.h"

enum EApplication {
	_eApplicationBegin_ = GETLAYERID(__eApplication__),
	_eBootStrapper_,
	_eLifecycleManager_,
	_eApplicationEnd_
};

#endif
