
#include "MemoryManager.h"
#include "../../01Global/MemoryManager/Memory.h"

MemoryManager::MemoryManager(const char* className, size_t slotSIze, unsigned numOfSlots) :
	name(className), slotSIze(slotSIze), numOfSlots(numOfSlots), freeSlotHead(NULL), count(0)
{
	// allocate a real memory
	this->segmentSize = numOfSlots * slotSIze;
	pStartAddress = reinterpret_cast <char *>(Memory::sDynamicSegment.allocate(this->segmentSize));
}

MemoryManager::~MemoryManager() {}

void MemoryManager::initialize() {
	this->allocateASegment();
}

void MemoryManager::finalize() {}

void MemoryManager::allocateASegment() {
	if (this->pStartAddress == NULL) {
		throw eNotInitialized;
	}
	// initialize nodes
	this->freeSlotHead = reinterpret_cast<FreeSlot*>(this->pStartAddress);
	FreeSlot* pCurrentSlot = this->freeSlotHead;

	// compute slot size
	rsize_t newSlotSize = (this->slotSIze > sizeof(FreeSlot)) ? this->slotSIze : sizeof(FreeSlot);

	// allocate Slots until the end of this page
	for (char *pNextAddress = this->pStartAddress + newSlotSize;
		pNextAddress + newSlotSize <= this->pStartAddress + this->segmentSize;
		pNextAddress = pNextAddress + newSlotSize)
	{
		pCurrentSlot->next = reinterpret_cast<FreeSlot*>(pNextAddress);
		pCurrentSlot = pCurrentSlot->next;
	}
	pCurrentSlot->next = 0;
}

void* MemoryManager::allocate() {
	if (this->freeSlotHead == NULL) {
		throw eNoMoreFreeSlot;
	}
	this->count++;
	FreeSlot* pFreeSlot = this->freeSlotHead;
	this->freeSlotHead = this->freeSlotHead->next;
	return pFreeSlot;
}

void MemoryManager::free(void* pDeleted) {
	if (pDeleted == NULL) {
		throw static_cast<int>(eDeleteNullPointer);
	}
	this->count--;
	FreeSlot* pDeletedSlot = reinterpret_cast<FreeSlot*>(pDeleted);
	pDeletedSlot->next = this->freeSlotHead;
	this->freeSlotHead = pDeletedSlot;
}