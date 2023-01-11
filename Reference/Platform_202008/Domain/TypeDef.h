#pragma once

#include "../../../Core_Lib/Source/GlobalTypeDef.h"

enum class EDomain {
	eBegin = GET_LAYER_ID(ELayer::eDomain),
	eSource,
	eImageProcessor1,
	eImageProcessor1Part,
	eImageProcessor2,
	eEnd
};
