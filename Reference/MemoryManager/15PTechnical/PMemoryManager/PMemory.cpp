//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "PMemory.h"

#include "../../01Global/MemoryManager/StaticSegment.h"
#include "../../15PTechnical/PMemoryManager/PDynamicSegment.h"

static char staticMemoryAllocated[STATICSEGMENTSIZE];
Segment StaticSegment::staticSegment("Static Segment", staticMemoryAllocated, STATICSEGMENTSIZE);
void StaticSegment::initialize() {
	staticSegment.initialize();
}


static char dynamicMemoryAllocated[DYNAMICSEGMENTSIZE];
Segment DynamicSegment::dynamicSegment("Dynamic Segment", dynamicMemoryAllocated, DYNAMICSEGMENTSIZE);

DynamicSegment *Message::pDynamicSegment = NULL;
DynamicSegment *Argument::pDynamicSegment = NULL;

void DynamicSegment::initialize() {
	dynamicSegment.initialize();

	// dynamic memory allocation
	Message::pDynamicSegment = new PDynamicSegment("MsgMemoryManager", sizeof(Message), MAXMSGCOUNT);
	Message::pDynamicSegment->initialize();

	Argument::pDynamicSegment = new PDynamicSegment("MsgMemoryManager", sizeof(Message), MAXMSGCOUNT);
	Argument::pDynamicSegment->initialize();

}
