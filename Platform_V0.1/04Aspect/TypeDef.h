//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef ASPECT_TYPEDEF
#define ASPECT_TYPEDEF  "ASPECT_TYPEDEF"
#include "../01Global/TypeDef.h"

enum EAspect {
	_eAspectBegin_ = GETLAYERID(__eAspect__),
	_eLogManager_,
	_eExceptionManager_,
	_eAspectEnd_
};

#endif