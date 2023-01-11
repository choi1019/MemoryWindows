#pragma once
#include "../TypeDef.h"
#define HEAPSTATIC "HeapStatic"  // class string name
#define HEAPSTATIC_ID GET_MODULE_ID(EGlobal::eHeapDynamic)  // class int id

class HeapStatic {
public:
	enum EErrorCode {
		eBegin = HEAPSTATIC_ID,
		eNotInitialized,
		eNoMoreFreeSlot,
		eDeleteNullPointer,
		eNotImplemented,
		eEnd
	};

	static HeapStatic *spHeapStatic;

	HeapStatic(size_t sizeHeap, const char* pName = nullptr) :
		pCurrentAddress((SlotTag*)spStartAddress), sizeHeap(sizeHeap),
		pName(pName), sizeSlotTag(sizeof(SlotTag)), count(0)
	{
	}

	~HeapStatic() {
	}

	void initialize() {}
	void finalize() {}

	void* malloc(size_t sizeSlot) {
		if (((size_t)this->pCurrentAddress + this->sizeSlotTag + sizeSlot) > ((size_t)spStartAddress + this->sizeHeap)) {
//			printf("HeapStatic::malloc() - no more memnoty");
			throw eNoMoreFreeSlot;
		}
		this->count++;

		// allocate memory
		void *pMemoryAllocated = (void *)((size_t)pCurrentAddress + this->sizeSlotTag);
		// store allocated slot info at pCurrentAddress
		this->pCurrentAddress->allocated = true;
		this->pCurrentAddress->size = sizeSlot;
		// increment pCurrentAddress
		this->pCurrentAddress = (SlotTag *)((size_t)this->pCurrentAddress  + this->sizeSlotTag + sizeSlot);

//		printf("HeapStatic::%s::malloc() - %d: %d(%d)\n",
//			pName, (size_t)pMemoryAllocated, sizeSlot, (size_t)pMemoryAllocated + sizeSlot - (size_t)spStartAddress);
		return pMemoryAllocated;
	}

	void free(void* pDeleted) {
		if (pDeleted == nullptr) {
			throw static_cast<int>(eDeleteNullPointer);
		}
		this->count--;
//		cout << "HeapStatic(" << pName << ")::free(" << pDeleted << ")\n";
		((SlotTag*)pDeleted)->allocated = false;
	}

	void* operator new(size_t size, void *pHeapStatic) {
		spStartAddress = (SlotTag*)pHeapStatic;
		spStartAddress = (SlotTag*)((size_t)spStartAddress + size);
		return pHeapStatic;
	}

	void operator delete(void *ptr) {
//		cout << "Error - HeapStatic::delete()\n";
		HeapStatic::spHeapStatic->free(ptr);
	}

	void operator delete(void* ptr, void* pHeapStatic) {
//		cout << "Error - HeapStatic::delete()\n";
		HeapStatic::spHeapStatic->free(ptr);
	}

private:
	struct SlotTag
	{
		bool allocated;
		size_t size;
	};
	size_t sizeSlotTag;

	// for debugging
	const char* pName;
	unsigned count;   // for test memory leakage

	static void* spStartAddress;
	SlotTag* pCurrentAddress;
	size_t sizeHeap;

};