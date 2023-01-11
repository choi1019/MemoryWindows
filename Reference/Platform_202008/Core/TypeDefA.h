#pragma once

#include "../GlobalTypeDef.h"

enum class EApplication {
	eBegin = GET_LAYER_ID(ELayer::eApplication),
	eMain,
	eLifecycleManager,
	eEnd
};