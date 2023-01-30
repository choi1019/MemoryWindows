#pragma once

#include <01Base/typedef.h>
#define _Collection_Id _GET_CLASS_UID(_ELayer_Base::_eCollection)
#define _Collection_Name "Collection"

#include <01Base/Aspect/Exception.h>
#include <01Base/Object/BaseObject.h>

class Collection : public BaseObject {
public:
	enum class EError
	{
		_eBegin = _Collection_Id,
		_eIndexOverflow,
		_eEnd
	};

public:
	Collection(int nClassId = _Collection_Id, const char* cClassName = _Collection_Name)
		: BaseObject(nClassId, cClassName)
	{
	}
	virtual ~Collection() {
	}
	virtual void Initialize() {
		BaseObject::Initialize();
	}
	virtual void Finalize() {
		BaseObject::Finalize();
	}
};