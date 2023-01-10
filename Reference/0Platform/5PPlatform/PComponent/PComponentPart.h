#pragma once

#include "../TypeDef.h"
#define PComponentPart_Id _GET_CLASS_UID(_EPPlatform::_ePComponentPart)
#define PComponentPart_Name "PComponentPart"  // class string name

#include "../../2Platform/Component/ComponentPart.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define PRIORITY 100

class PComponentPart : public ComponentPart {

private:
	HANDLE m_hThread;
	DWORD m_dwThreadId;

protected:
	void RegisterExceptions();

public:
	// for Component
	PComponentPart(
		unsigned uClassId = PComponentPart_Id,
		const char* acClassName = PComponentPart_Name);
	virtual ~PComponentPart();
	virtual void Initialize();
	virtual void Finalize();

	virtual void Start();
	virtual void Stop();
	virtual void Pause();

	virtual void Run();
};