#pragma once

#define UNDEFINED -1

#define GET_LAYER_ID(eLayerID) (unsigned)eLayerID*1000
#define GET_MODULE_ID(eModuleID) (unsigned)eModuleID*1000

enum class ELayer {
	eBegin,
	eGloabal = 1,
	eUtility = 2,
	eCore = 3,
	eAspect = 4,
	eTechnical = 5,
	eDomain = 7,
	eApplication = 9,

	ePGlobal = 11,
	ePUtility = 12,
	ePCore = 13,
	ePAspect = 14,
	ePTechnical = 15,
	ePDomain = 17,
	ePApplication = 19,
	ePProduct = 20,
	eEnd
};

//#include <stdio.h>

#include "01Utility/collection/map.h"
#include "01Utility/collection/vector.h"
using namespace nibble;

#define PRINTLN(string) \
this->SendAEvent(\
	(unsigned)Component::EReceivers::eDisplay,\
	(unsigned)Display::EEventType::ePrintln,\
	0,\
	(ValueObject*)string\
);\

#define SIZE_MEMORYALLOCATED 4900
#define NUM_UIDSLOTS 5
#define NUM_EVENTSLOTS 15

#define MAXCOMPONENTPARTS		3
#define MAXRECEIVERCOMPONENTS	3
#define MAXTARGETGROUPS			2
#define MAXTARGETCOMPONENTS		3

#define MAXQUEUESIZE			5

#define MAXCOMPONENTSALLOCATED	3
