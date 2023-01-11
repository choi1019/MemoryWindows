//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////
#include "../PMemoryManager/PDynamicSegment.h"

PDynamicSegment::PDynamicSegment(const char* name, int slotSIze, int numOfSlots)
	: DynamicSegment(name, slotSIze, numOfSlots)
{
	// default security attributes, initial count, maximum count, name
	InitializeSRWLock(&SRWLock);
}

PDynamicSegment::~PDynamicSegment()
{
}

void PDynamicSegment::initialize()
{
	DynamicSegment::initialize();;
}

void PDynamicSegment::finalize()
{
}

void* PDynamicSegment::safeAllocate()
{
	/////////////////////////////////////////////////////////////
	// critical section
	AcquireSRWLockExclusive(&SRWLock);
	void* pAllocated = this->allocate();
	ReleaseSRWLockExclusive(&SRWLock);
	/////////////////////////////////////////////////////////////
	return pAllocated;
}

void PDynamicSegment::safeFree(void *pDeleted)
{
	/////////////////////////////////////////////////////////////
	// critical section
	AcquireSRWLockExclusive(&SRWLock);
	this->free(pDeleted);
	ReleaseSRWLockExclusive(&SRWLock);
	/////////////////////////////////////////////////////////////
}
