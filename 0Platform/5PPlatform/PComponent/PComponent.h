#pragma once

#include "../typedef.h"
#define _PCompnent_Id _GET_CLASS_UID(_EPPlatform::_ePComponent)
#define _PCompnent_Name "PCompnent"

#include "../../2Platform/Component/Component.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define PRIORITY 100

class PComponent : public Component
{
private:
	HANDLE m_hThread;
	DWORD m_dwThreadId;

public:
	PComponent(int nClassId = _PCompnent_Id, const char* pcClassName = _PCompnent_Name);
	virtual ~PComponent();
	virtual void Run();
	virtual void Start();

protected:
	virtual void RegisterEventTypes();
	virtual void RegisterExceptions();

	virtual void Initialize();
	virtual void Finalize();

	virtual void Stop();
	virtual void Pause();
};