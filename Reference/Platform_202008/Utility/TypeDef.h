#pragma once

#include "../GlobalTypeDef.h"

enum class EUtility {
	eBegin = GET_LAYER_ID(ELayer::eUtility),
	eVector,
	eQueue,
	eMap,
	eString,
	eEnd
};
