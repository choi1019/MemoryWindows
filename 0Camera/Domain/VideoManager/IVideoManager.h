#pragma once

#include "../typedef.h"
#define _VideoManager_Id _GET_CLASS_UID(_ELayer_Domain::_eVideoManager)
#define _VideoManager_Name "VideoManager"

class IVideoManager {
public:
	////////////////////////////////////////
	// Required interface
	////////////////////////////////////////
	enum class EReceivers {
		eBegin = _VideoManager_Id,
		eVideoInput,
		eVideoOutput,
		eEnd
	};
	enum class ESources {
		eBegin = (int)EReceivers::eEnd,
		eTimer,
		eEnd
	};

	///////////////////////////////////////
	// Provuded Interface
	///////////////////////////////////////
	// event type
	enum class EEventType {
		eBegin = _VideoManager_Id,
		eEnd
	};
};

