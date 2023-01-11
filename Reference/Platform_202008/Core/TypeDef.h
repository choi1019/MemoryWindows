#pragma once

#include "../GlobalTypeDef.h"

enum class ECore {
	eBegin = GET_LAYER_ID(ELayer::eCore),
	eComponentPart,
	eComponent,
	eScheduler,
	eEventQueue,
	eEvent,
	eUId,
	eEnd
};
