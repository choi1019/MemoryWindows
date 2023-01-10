#pragma once

#include "../typedef.h"
#define _PMain_Id _GET_CLASS_UID(_EPApplication::_ePMain)
#define _PMain_Name "PMain"

#include "../../../Platform/2Platform/Main/Main.h"
#include "../../../Platform/3Technical/MemoryManager/MemoryManager.h"
#include <Windows.h>
#undef GetClassName

class PMain : public Main
{
public:
	PMain(unsigned uClassId = _PMain_Id,
		const char* pcClassName = _PMain_Name);
	virtual ~PMain();

	void AddParts(MemoryManager* pMemoryManager);
	virtual void BootstrapSystem();

	virtual void CloseSystem();
};