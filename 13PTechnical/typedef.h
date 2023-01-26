
#ifndef _ELAYER_PTECHNICAL
#define _ELAYER_PTECHNICAL

#include <typedef.h>

enum class _ELayer_PTechnical {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePTechnical),

	_ePMemory,
	_ePMemoryManager,
	_ePMemoryVariable,
	_ePMemoryEven,

	_ePTimer,
	_ePSocketServer,
	_ePSocketServerWorker,
	_ePSocketClient,

	_eRSTP,

	_eEnd
};

#endif

