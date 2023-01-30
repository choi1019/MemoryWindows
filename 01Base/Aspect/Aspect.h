#pragma once

#include <01Base/typedef.h>
#define _Aspect_Id _GET_CLASS_UID(_ELayer_Base::_eAspect)
#define _Aspect_Name "Aspect"

#include <01Base/Object/RootObject.h>

class Aspect : public RootObject
{
public:
	Aspect(unsigned classId = _Aspect_Id,
		const char* pClassName = _Aspect_Name)
		: RootObject(classId, pClassName)
	{
	}
	virtual ~Aspect() {}

	virtual void Initialize() {
		RootObject::Initialize();
	}
	virtual void Finalize() {
		RootObject::Finalize();
	}
	void PrintNextLine() {
		printf("%s\n", Aspect::GetTab());
	}
	void PrintSeparator() {
		printf("\n%s-------------------------------------------------", this->GetTab());
	}


public:
	static unsigned s_uCountTab;
	static char s_pcTab[];
	static char* GetTab();
	static void AddTab();
	static void RemoveTab();
};

