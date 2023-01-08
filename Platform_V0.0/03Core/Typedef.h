//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef CORE_TYPEDEF
#define CORE_TYPEDEF  "CORE_TYPEDEF"
#include "../../01Global/TypeDef.h"

enum ECore {
	_eCoreBegin_ = GETLAYERID(__eCore__),
	_eComponent_,
	_eComponentMsgVector_,
	_eMessage_,
	_eUId_,
	_eLog_,
	_eTask_,
	_eTaskMsgQueue_,
	_eCoreEnd_
};

#endif /* CORE_TYPEDEF */