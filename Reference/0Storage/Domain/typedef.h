#pragma once

#include "../../Platform/1Base/typedef.h"

enum class _ELayer_Domain {
	_eBegin = _GET_LAYER_UID(_ELayer::_eDomain),

	_eSender,
	_eReceiver,

	_eTimer,
	_eTarget1,
	_eTarget2,

	_eEnd
};



