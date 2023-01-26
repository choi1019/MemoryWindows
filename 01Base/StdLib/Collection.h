#ifndef COLLECTION
#define COLLECTION

#include <01Base/typedef.h>
#define _COLLECTION_Id _GET_CLASS_UID(_ELayer_Base::_eCollection)
#define _COLLECTION_Name "Collection"

#include <01Base/Aspect/Exception.h>
#include <01Base/Object/BaseObject.h>

class Collection : public BaseObject {
public:
	enum class EError
	{
		_eBegin = _COLLECTION_Id,
		_eIndexOverflow,
		_eEnd
	};

public:
	Collection(int nClassId = _COLLECTION_Id, const char* cClassName = _COLLECTION_Name)
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

#endif