#pragma once

#include "../../../Core_Lib/Source/GlobalTypeDef.h"

enum class EPDomain {
	eBegin = GET_LAYER_ID(ELayer::ePDomain),
	ePSource,
	ePImageProcessor1,
	ePImageProcessor1Part,
	ePImageProcessor2,
	eEnd
};
