#pragma once

#include "../../../Core_Lib/Source/GlobalTypeDef.h"

enum class EPCore {
	eBegin = GET_LAYER_ID(ELayer::ePCore),
	ePEventQueue,
	ePScheduler,
	eEnd
};
