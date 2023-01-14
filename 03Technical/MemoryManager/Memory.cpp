#include "Memory.h"

void* Memory::s_pMemoryAllocated = nullptr;
size_t Memory::s_szThis = 0;

void* SlotManager::s_pMemoryAllocated = nullptr;
size_t SlotManager::s_szThis = 0;

void* PageManager::s_pMemoryAllocated = nullptr;
size_t PageManager::s_szThis = 0;