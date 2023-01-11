//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Are Reserved
//////////////////////////////////////////////////////////////

#ifndef OBJECT
#define OBJECT "Object"
#include "../TypeDef.h"
#define OBJECT_ID GETCLASSID(_eObject_)

#include "../../01Global/MemoryManager/Memory.h"

class Object {
public:
	enum EErrorCode {
		eErrorCodeBegin = OBJECT_ID,
		eNotSupported,
		eErrorCodeEnd
	};

private:
	// unique object id generator
	static unsigned sObjectId;

	const unsigned objectId;
	const ClassInfo classInfo;

public:
	// constructors
	Object(ClassInfo classInfo) : objectId(sObjectId++), classInfo(classInfo) {}
	virtual ~Object() {}

	virtual void initialize() {}
	virtual void finalize() {}

	inline const int getObjectId() { return this->objectId; }
	inline const ClassInfo getClassInfo() { return this->classInfo; }
	inline const int getClassId() { return this->classInfo.id; }
	inline const char* getClassName() { return this->classInfo.pName; }

	//////////////////////////////////////////////////////////////////////
	// static memory allocator
	inline void *operator new (size_t size) {
		return Memory::sStaticSegment.allocate(size);
	}
	inline void operator delete(void *pObject) {
//		throw eNotSupported;
	}
	//////////////////////////////////////////////////////////////////////
};

#endif
