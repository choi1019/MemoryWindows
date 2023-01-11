//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef MEMORY
#define MEMORY "Memory"
#include "../TypeDef.h"
#define MEMORY_ID GETCLASSID(_eMemory_)

#include "../../01Global/MemoryManager/Segment.h"

class StaticSegment {
public:
	static Segment staticSegment;
	static void initialize();
};

#endif