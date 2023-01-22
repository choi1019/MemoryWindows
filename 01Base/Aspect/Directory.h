#pragma once

#include "../typedef.h"
#define _Directory_Id _GET_CLASS_UID(_ELayer_Base::_eDirectory)
#define _Directory_Name "Exception"

#include "../../01Base/StdLib/Map.h"
#include "../../01Base/Aspect/Log.h"
#undef GetClassName

typedef Map<size_t, String> DirectoryMap;
class Directory : public DirectoryMap
{
public:
	enum class EType {
		eObject,
		eComponent,
		eScheduler,
		eEvent,
		eException,
		eLog,
		eEnd
	};

private:
	EType m_eType;

public:
	static Directory s_dirObjects;
	static Directory s_dirComponents;
	static Directory s_dirEvents;
	static Directory s_dirExceptions;

	Directory(EType eType)
		: DirectoryMap(_Directory_Id, _Directory_Name)
		, m_eType(eType)
	{
	}
	virtual ~Directory() {}

	virtual void Initialize() {
		DirectoryMap::Initialize();
	}
	virtual void Finalize() {
		DirectoryMap::Finalize();
	}

	void Show(const char* pcTitle) {
		LOG_HEADER("Directory", (int)m_eType);
		for (auto itr : *this) {
			LOG_NEWLINE(itr.first, itr.second);
		}
		LOG_FOOTER("Directory", (int)m_eType);
	}
};

static void ClearDirectories() {
	Directory::s_dirObjects.Clear();
	Directory::s_dirComponents.Clear();
	Directory::s_dirEvents.Clear();
	Directory::s_dirExceptions.Clear();
};


