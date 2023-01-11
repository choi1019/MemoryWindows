#pragma once

#include "../../../Core_Lib/Source/GlobalTypeDef.h"

enum class EPApplication {
	eBegin = GET_LAYER_ID(ELayer::ePApplication),
	ePMain,
	ePLifecycleManager,
	eEnd
};
