#include "Memory.h"

void* Memory::s_pMemoryAllocated = nullptr;
size_t Memory::s_sizeThis = 0;

void* SlotManager::s_pMemoryAllocated = nullptr;
size_t SlotManager::s_sizeThis = 0;