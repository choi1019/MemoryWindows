//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PTECHNICAL_TYPEDEF
#define PTECHNICAL_TYPEDEF "PTECHNICAL_TYPEDEF"
#include "../01Global/TypeDef.h"

enum EPTechnical {
	_ePTechnicalBegin_ = GETLAYERID(__ePTechnical__),
	_ePMemory_, 
	_ePMemoryManager_,
	_ePTask_,
	_ePTaskMsgQueue_,
	_ePComponentMsgVector_,
	_ePTimer_,
	_ePTechnicalLayerEnd_
};

#include <windows.h>

// Memory Allocation
#define STATICSEGMENTSIZE 40000
#define DYNAMICSEGMENTSIZE 30000

// ValueObject
#define MAXMSGCOUNT 100
#define MAXLOGCOUNT 100
#define MAXRECEIVERPARAMCOUNT 100

// Task Message Queue
#define MAXTASKMSGQUEUELENGTH 100
// Component Message Vector
#define MAXCOMPONENTMSGVECTORLENGTH 50

#endif

