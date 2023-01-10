#pragma once

#include "../../typedef.h"
#define _PTC41_SENDER_NAME_ "ISender"
#define _PTC41_SENDER_ID_ _GET_CLASS_UID_(_EPTestApplication::_ePTC41_Sender)

class ISender {
public:
	////////////////////////////////////////
	// Component Parts
	////////////////////////////////////////
	enum class EPart {
		eBegin = _PTC41_SENDER_ID_,
		eSenderPart,
		eEnd
	};

	////////////////////////////////////////
	// Required interface
	////////////////////////////////////////
	enum class EReceivers {
		eBegin = _PTC41_SENDER_ID_,
		eReceiver,
		eEnd
	};

	///////////////////////////////////////
	// Provuded Interface
	///////////////////////////////////////
	// event type
	enum class EEventType {
		eBegin = _PTC41_SENDER_ID_,
		eStart,
		eEnd
	};
};

