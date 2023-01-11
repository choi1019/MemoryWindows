#pragma once

#include "../../../Core_Lib/Source/GlobalTypeDef.h"

enum class EPTechnical {
	eBegin = GET_LAYER_ID(ELayer::ePTechnical),
	ePTimer,
	eEnd
};

#define MAX_STATICHEAP_SIZE 2048