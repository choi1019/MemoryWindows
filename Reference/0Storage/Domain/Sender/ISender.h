#pragma once

#include "../typedef.h"
#define _Sender_Id _GET_CLASS_UID(_ELayer_Domain::_eSender)
#define _Sender_Name "Sender"

class ISender {
public:
	////////////////////////////////////////
	// Required interface
	////////////////////////////////////////
	enum class EReceivers {
		eBegin = _Sender_Id,
		eReceiver,
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
		eBegin = _Sender_Id,
		eEnd
	};
};

