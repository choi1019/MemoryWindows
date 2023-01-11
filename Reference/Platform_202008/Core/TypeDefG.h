#pragma once

#include "../GlobalTypeDef.h"

enum class EGlobal {
	eBegin = GET_LAYER_ID(ELayer::eGloabal),
	eHeapDynamic,
	eHeapStatic,
	eValueObject,
	eObject,
	eEnd
};
