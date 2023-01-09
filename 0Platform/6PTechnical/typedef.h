#pragma once

#include "../1Base/typedef.h"

enum class _ELayer_PTechnical {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePTechnical),


	_ePTimer,
	_ePSocketServer,
	_ePSocketServerWorker,
	_ePSocketClient,

	_eRSTP,

	_eEnd
};



