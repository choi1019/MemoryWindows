#pragma once
#include "../TypeDef.h"
#define PMEMORY "PMemory"  // class string name
#define PMEMORY_ID GET_MODULE_ID(EPGlobal::ePMemory)  // class int id

class PMemory {
public:
	static void allocate();
};
