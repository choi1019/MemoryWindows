//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef GLOBAL_TYPEDEF
#define GLOBAL_TYPEDEF "GLOBAL_TYPEDEF"

#define NULL 0
#define NOT_FOUND -1
#define UNDEFINED -1

#define GETLAYERID(LAYERID) LAYERID * 100
#define GETCLASSID(CLASSID) CLASSID * 100

enum EPlatformLayers {
	__ePlatformLayersBegin__ = 0,
	__eGlobal__ = 1,
	__eUtility__ = 2,
	__eCore__ = 3,
	__eAspect__ = 4,
	__eTechnical__ = 5,
	__eDomain__ = 6,
	__eApplication__ = 7,
	__ePlatformLayersEnd__
};

enum EProductLayers {
	__eProductLayersBegin__ = __ePlatformLayersEnd__,
	__ePAspect__ = __eAspect__ + 10,
	__ePTechnical__ = __eTechnical__ + 10,
	__ePDomain__ = __eDomain__ + 10,
	__ePApplication__  = __eApplication__ + 10,
	__eProductLayersEnd__
};

enum EGlobal {
	_eGlobalBegin_ = GETLAYERID(__eGlobal__),
	_eMemory_,
	_eSegment_,
	_eMemoryManager_, 
	_eObject_,
	_eValueObject_,
	_eGlobalEnd_
};

class ClassInfo {
public:
	const unsigned id;
	const char *pName;
	ClassInfo(const unsigned id, const char* pName) : id(id), pName(pName) {}
};

static ClassInfo Make_ClassInfo(const unsigned id, const char* pName) {
	ClassInfo info(id, pName);
	return info;
}

//#define NDEBUG
//#include <assert.h>
//#include <iostream>
//using namespace std;
#include <stdio.h>

#endif /* GLOBAL_TYPEDEF */