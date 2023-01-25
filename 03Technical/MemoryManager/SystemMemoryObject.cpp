#include "SystemMemoryObject.h"

void* SystemMemoryObject::s_pSystemMemoryAllocated = nullptr;
void* SystemMemoryObject::s_pCurrentSystemMemoryAllocated = nullptr;
size_t SystemMemoryObject::s_szSystemMemoryAllocated = 0;