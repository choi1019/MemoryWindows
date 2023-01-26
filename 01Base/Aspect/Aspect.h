#pragma once

#include <01Base/typedef.h>
#define _Aspect_Id _GET_CLASS_UID(_ELayer_Base::_eAspect)
#define _Aspect_Name "Aspect"

#include <01Base/Object/RootObject.h>
#include <stdio.h>

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
		printf("%s\n", this->GetTab());
	}
	void PrintSeparator() {
		printf("\n%s-------------------------------------------------", this->GetTab());
	}


public:
	/////////////////////////////////////////////////
	// pretty formatting
	/////////////////////////////////////////////////
#define COUNT_TAB 20
#define SIZE_TAB 4
#define SPACE ' '

	static unsigned s_uCountTab;
	static char s_pcTab[];
	static char* GetTab() {
		for (unsigned i = 0; i < s_uCountTab * SIZE_TAB; i++) {
			s_pcTab[i] = SPACE;
		}
		s_pcTab[s_uCountTab * SIZE_TAB] = '\0';
		return s_pcTab;
	}
	static void AddTab() { s_uCountTab++; }
	static void RemoveTab() { s_uCountTab--; }
};

