#pragma once

#include "../1Base/typedef.h"

enum class _EPPlatform {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePPlatform),

	_ePComponent,
	_ePComponentPart,
	_ePEventQueue,
	_ePScheduler,

	_ePMemoryManager,
	_ePMemoryVariable,
	_ePMemoryEven,

	_eEnd
};
