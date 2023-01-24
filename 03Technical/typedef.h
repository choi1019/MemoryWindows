#pragma once

#include "../01Base/typedef.h"

enum class _ELayer_Technical {
	_eBegin = _GET_LAYER_UID(_ELayer::_eTechnical),

	_eMemoryStatic,
	_eMemoryManager,
	_eMemoryDynamic,
	_eMemoryVariable,
	_eMemoryEven,

	_eTimer,
	_eSocketServer,
	_eSocketClient,
	_eParser,

	_eEnd
};

#include "../01Base/Aspect/Exception.h"
#include "../01Base/Aspect/Log.h"

