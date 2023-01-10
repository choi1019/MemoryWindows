//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "PMemory.h"

void Memory::initialize() {
	Memory::initializeStaticSegment();
	Memory::initializeDynamicSegment();
}

///////////////////////////////////////////////////////////////////////////// 
// Static Segment
/////////////////////////////////////////////////////////////////////////////
static char staticMemoryAllocated[STATICSEGMENTSIZE];
Segment Memory::sStaticSegment("Static Segment", staticMemoryAllocated, STATICSEGMENTSIZE);

void Memory::initializeStaticSegment() {
	Memory::sStaticSegment.initialize();
}

/////////////////////////////////////////////////////////////////////////////
// Dynamic Segment
/////////////////////////////////////////////////////////////////////////////
static char dynamicMemoryAllocated[DYNAMICSEGMENTSIZE];
Segment Memory::sDynamicSegment("Dynamic Segment", dynamicMemoryAllocated, DYNAMICSEGMENTSIZE);

void Memory::initializeDynamicSegment() {
	Memory::sDynamicSegment.initialize();
	Memory::initializeValueObjects();
}

/////////////////////////////////////////////////////////////////////////////
// ValueObject memory allocation
/////////////////////////////////////////////////////////////////////////////
#include "../../15PTechnical/PMemoryManager/PMemoryManager.h"
#include "../../03Core/Component/Message.h"


void Memory::initializeValueObjects() {
	Message::pMemoryManager = new PMemoryManager("MsgMemoryManager", sizeof(Message), MAXMSGCOUNT);
	Message::pMemoryManager->initialize();

	/*
	Log::pMemoryManager = new PMemoryManager("LogMemoryManager", sizeof(Log), MAXLOGCOUNT);
	Log::pMemoryManager->initialize();

	Receiver::SFunctionParam::pMemoryManager = new PMemoryManager("ParamMemoryManager", sizeof(Receiver::SFunctionParam), MAXRECEIVERPARAMCOUNT);
	Receiver::SFunctionParam::pMemoryManager->initialize();
	*/
}