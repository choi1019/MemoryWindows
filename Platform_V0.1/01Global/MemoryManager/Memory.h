//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef MEMORY
#define MEMORY "Memory"
#include "../TypeDef.h"
#define MEMORY_ID GETCLASSID(_eMemory_)

#include "../../01Global/MemoryManager/Segment.h"

class Memory {
public:
	static Segment sStaticSegment;
	static Segment sDynamicSegment;

	static void initialize();
	static void initializeStaticSegment();
	static void initializeDynamicSegment();
	static void initializeValueObjects();
};

#endif