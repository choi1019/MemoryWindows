//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PMEMORYMANAGER
#define PMEMORYMANAGER  "PMemoryManager"
#include "../typedef.h"

#include "../../01Global/MemoryManager/DynamicSegment.h"

class PDynamicSegment : public DynamicSegment {
private:
	SRWLOCK SRWLock;
public:
	PDynamicSegment(const char* name, int slotSIze, int numOfSlots = 64);
	virtual ~PDynamicSegment();
	void initialize();
	void finalize();

	void* safeAllocate();
	void safeFree(void *pDeleted);
};

#endif