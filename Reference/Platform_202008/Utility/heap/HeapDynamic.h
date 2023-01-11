#pragma once
#include "../TypeDef.h"
#define HEAPDYNAMIC "HeapDynamic"  // class string name
#define HEAPDYNAMIC_ID GET_MODULE_ID(EGlobal::eHeapDynamic)  // class int id

#include "HeapStatic.h"

class HeapDynamic {
public:
	enum EErrorCode {
		eBegin = HEAPDYNAMIC_ID,
		eNotInitialized,
		eNoMoreFreeSlot,
		eDeleteNullPointer,
		eNotImplemented,
		eEnd
	};

	HeapDynamic(size_t sizeSlot, unsigned numSlots, const char* pName = nullptr) :
		sizeSlot(sizeSlot), numSlots(numSlots), sizeSegment(sizeSlot* numSlots),
		pName(pName), count(0), freeSlotHead(nullptr)
	{

		this->pStartAddress = (char *)HeapStatic::spHeapStatic->malloc(sizeSegment);
		if (this->pStartAddress == nullptr) {
			throw eNotInitialized;
		}
//		printf("HeapDynamic::%s - %d: %dx%d\n", pName, (size_t)pStartAddress, sizeSlot, numSlots);
		// initialize nodes
		this->freeSlotHead = reinterpret_cast<FreeSlot*>(this->pStartAddress);
		FreeSlot* pCurrentSlot = this->freeSlotHead;

		// compute slot size
		size_t newSlotSize = (this->sizeSlot > sizeof(FreeSlot)) ? this->sizeSlot : sizeof(FreeSlot);

		// allocate Slots until the end of this page
		for (char* pNextAddress = this->pStartAddress + newSlotSize;
			pNextAddress + newSlotSize <= this->pStartAddress + this->sizeSegment;
			pNextAddress = pNextAddress + newSlotSize)
		{
			pCurrentSlot->next = reinterpret_cast<FreeSlot*>(pNextAddress);
			pCurrentSlot = pCurrentSlot->next;
		}
		pCurrentSlot->next = 0;
	}

	~HeapDynamic() {
		HeapStatic::spHeapStatic->free(this->pStartAddress);
	}

	virtual void initialize() {
	}
	virtual void finalize() {}

	virtual void* malloc(size_t size) {
		if (this->freeSlotHead == nullptr) {
//			printf("HeapDynamic::malloc() - no more memnoty");
			throw eNoMoreFreeSlot;
		}
		this->count++;
		FreeSlot* pFreeSlot = this->freeSlotHead;
		this->freeSlotHead = this->freeSlotHead->next;
//		cout << "HeapDynamic(" << pName << ")::malloc (" << pFreeSlot << ", " << size << ", " << count << ")\n";
		return pFreeSlot;
	}
	virtual void free(void* pDeleted) {
		if (pDeleted == nullptr) {
			throw static_cast<int>(eDeleteNullPointer);
		}
		this->count--;
		FreeSlot* pDeletedSlot = reinterpret_cast<FreeSlot*>(pDeleted);
		pDeletedSlot->next = this->freeSlotHead;
		this->freeSlotHead = pDeletedSlot;
//		cout << "HeapDynamic(" << pName << ")::free (" << pDeleted << ", " << count << ")\n";
	}

	void* operator new(size_t size) {
		void* pAllocated = HeapStatic::spHeapStatic->malloc(size);
		return pAllocated;
	}

	void operator delete(void* ptr) {
//		cout << "Error - HeapDynamic::delete()\n";
		HeapStatic::spHeapStatic->free(ptr);
	}

private:
	// for debugging
	const char* pName;
	unsigned count;   // for test memory leakage

	char* pStartAddress;
	size_t sizeSlot;
	unsigned numSlots;
	size_t sizeSegment;

	struct FreeSlot
	{
		FreeSlot* next;
	};
	FreeSlot* freeSlotHead;
};