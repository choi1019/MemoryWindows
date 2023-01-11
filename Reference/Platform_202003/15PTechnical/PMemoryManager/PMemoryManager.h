//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PMEMORYMANAGER
#define PMEMORYMANAGER  "PMemoryManager"
#include "../typedef.h"

#include "../../01Global/MemoryManager/MemoryManager.h"

class PMemoryManager : public MemoryManager {
private:
	SRWLOCK SRWLock;
public:
	PMemoryManager(const char* name, int slotSIze, int numOfSlots = 64);
	virtual ~PMemoryManager();
	void initialize();
	void finalize();

	void* safeAllocate();
	void safeFree(void *pDeleted);
};

#endif