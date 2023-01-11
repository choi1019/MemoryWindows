//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef MEMORYMANAGER
#define MEMORYMANAGER "DyanmicSegement"
#include "../TypeDef.h"
#define MEMORYMANAGER_ID GETCLASSID(_eMemoryManager_)

#include "../../01Global/MemoryManager/Segment.h"

class DynamicSegment
{
public:
	enum EErrorCode {
		eErrorCodeStart = MEMORYMANAGER_ID,
		eNotInitialized,
		eNoMoreFreeSlot,
		eDeleteNullPointer,
		eNotImplemented
	};

private:
	const char* name;
	unsigned count;   // for test memory leakage
	char* pStartAddress;
	size_t slotSIze;
	unsigned numOfSlots;

	size_t segmentSize;

	struct FreeSlot
	{
		FreeSlot *next;
	};
	FreeSlot* freeSlotHead;

	void allocateSlots();

public:
	static Segment dynamicSegment;

	DynamicSegment(const char* name, size_t slotSIze, unsigned numOfSlots);
	~DynamicSegment();

	virtual void initialize();
	virtual void finalize();

	void* allocate();
	void free(void* pDeleted);

	virtual void* safeAllocate() = 0;
	virtual void safeFree(void* pDeleted) = 0;

	//////////////////////////////////////////////////////////////////////
	// segment memory allocatoion
	inline void *operator new (size_t size) {
		return dynamicSegment.allocate(size);
	}
	//////////////////////////////////////////////////////////////////////
};

#endif