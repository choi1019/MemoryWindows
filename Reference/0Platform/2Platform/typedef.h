#pragma once

#include "../1Base/typedef.h"

enum class _EPlatform {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePlatform),

	_eUId,
	_eEvent,
	_eEventQueue,
	_eComponentPart,
	_eComponent,
	_eScheduler,
	_eLifecycleManager,
	_eMain,

	_eEnd
};

#define MAXCOMPONENTPARTS 20
#define MAXRECEIVERCOMPONENTS 20
#define MAXTARGETCOMPONENTS 10
#define MAXTARGETGROUPS 10