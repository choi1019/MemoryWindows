/////////////////////////////////////////////////////////////////
/// <summary>
/// Sungwoon Choi 2023-01-26
/// </summary>
/////////////////////////////////////////////////////////////////

#ifndef ROOTOBJECT
#define ROOTOBJECT

#include <01Base/typedef.h>

#define _ROOTOBJECT_Id _GET_CLASS_UID(_ELayer_Base::_eRootObject)
#define _ROOTOBJECT_Name "RootObject"
#undef GetClassName

class RootObject {
public:
	// constructors and destructors
	RootObject(unsigned nClassId = _ROOTOBJECT_Id, const char* pcClassName = _ROOTOBJECT_Name)
	{
	}
	virtual ~RootObject() 
	{
	}
	virtual void Initialize()
	{
	}
	virtual void Finalize()
	{
	}
};
#endif

