//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PASPECT_TYPEDEF
#define PASPECT_TYPEDEF "PASPECT_TYPEDEF"
#include "../01Global/TypeDef.h"

enum EPAspect {
	_ePAspectBegin_ = GETLAYERID(__ePAspect__),
	_ePExceptionManager_, 
	_ePAspectEnd_
};

#define EXCEPTION_FILENAME "exception.txt"

#endif

