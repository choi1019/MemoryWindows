#include "PMemory.h"

#include "../../../../Core_Lib/Source/00Global/Heap/HeapStatic.h"
#include "../../../../Core_Lib/Source/03Core/EventQueue/Event.h"
#include "../../../../Core_Lib/Source/03Core/Component/UId.h"

char memoryAllocated[SIZE_MEMORYALLOCATED];

void *HeapStatic::spStartAddress = nullptr;
HeapStatic *HeapStatic::spHeapStatic = nullptr;
HeapDynamic *Event::spMemoryManager = nullptr;
HeapDynamic *UId::spMemoryManager = nullptr;

void PMemory::allocate() {
	HeapStatic::spHeapStatic = new(memoryAllocated) HeapStatic(SIZE_MEMORYALLOCATED, "HeapStatic");

	Event::spMemoryManager = new HeapDynamic(sizeof(Event), NUM_EVENTSLOTS, "Event");
	UId::spMemoryManager = new HeapDynamic(sizeof(UId), NUM_UIDSLOTS, "UId");
}

