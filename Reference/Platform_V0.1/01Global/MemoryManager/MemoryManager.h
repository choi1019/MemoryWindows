//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef MEMORYMANAGER
#define MEMORYMANAGER "MemoryManager"
#include "../TypeDef.h"
#define MEMORYMANAGER_ID GETCLASSID(_eMemoryManager_)

#include "../../01Global/MemoryManager/Memory.h"
class MemoryManager
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

	void allocateASegment();

public:
	MemoryManager(const char* name, size_t slotSIze, unsigned numOfSlots);
	~MemoryManager();

	virtual void initialize();
	virtual void finalize();

	void* allocate();
	void free(void* pDeleted);

	virtual void* safeAllocate() = 0;
	virtual void safeFree(void* pDeleted) = 0;

	//////////////////////////////////////////////////////////////////////
	// static memory allocator
	inline void *operator new (size_t size) {
		return Memory::sDynamicSegment.allocate(size);
	}
	//////////////////////////////////////////////////////////////////////
};

#endif