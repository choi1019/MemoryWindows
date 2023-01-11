//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "../PMemoryManager/PMemoryManager.h"

PMemoryManager::PMemoryManager(const char* name, int slotSIze, int numOfSlots)
	: MemoryManager(name, slotSIze, numOfSlots)
{
	// default security attributes, initial count, maximum count, name
	InitializeSRWLock(&SRWLock);
}

PMemoryManager::~PMemoryManager()
{
}

void PMemoryManager::initialize()
{
	MemoryManager::initialize();;
}

void PMemoryManager::finalize()
{
}

void* PMemoryManager::safeAllocate()
{
	/////////////////////////////////////////////////////////////
	// critical section
	AcquireSRWLockExclusive(&SRWLock);
	void* pAllocated = this->allocate();
	ReleaseSRWLockExclusive(&SRWLock);
	/////////////////////////////////////////////////////////////
	return pAllocated;
}

void PMemoryManager::safeFree(void *pDeleted)
{
	/////////////////////////////////////////////////////////////
	// critical section
	AcquireSRWLockExclusive(&SRWLock);
	this->free(pDeleted);
	ReleaseSRWLockExclusive(&SRWLock);
	/////////////////////////////////////////////////////////////
}
