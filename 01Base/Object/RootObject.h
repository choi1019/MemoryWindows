#pragma once

#include <01Base/typedef.h>

#define _RootObject_Id _GET_CLASS_UID(_ELayer_Base::_eRootObject)
#define _RootObject_Name "RootObject"

class RootObject {
public:
	// constructors and destructors
	RootObject(unsigned nClassId = _RootObject_Id, const char* pcClassName = _RootObject_Name)
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

