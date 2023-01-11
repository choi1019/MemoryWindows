//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PMEMORY
#define PMEMORY "PMemory"
#include "../TypeDef.h"

#include "../../01Global/MemoryManager/StaticSegment.h"
#include "../../01Global/MemoryManager/DynamicSegment.h"

class ValueObject {
public:
	ValueObject() {
		printf("%s\n", __FUNCTION__);
	}
	virtual ~ValueObject() {
		printf("%s\n", __FUNCTION__);
	}
	inline void *operator new (size_t size, DynamicSegment *pMemoryManager) throw() {
		printf("%s\n", __FUNCTION__);
		return pMemoryManager->safeAllocate();
	}
	inline void destroy(DynamicSegment *pDynamicSegment) {
		printf("%s\n", __FUNCTION__);
		this->~ValueObject();
		pDynamicSegment->safeFree(this);
	}
};

class Message : public ValueObject {
public:
	static DynamicSegment *pDynamicSegment;
	Message() {
		printf("%s\n", __FUNCTION__);
	}
	~Message() {
		printf("%s\n", __FUNCTION__);
	}
private:
	int x;
	int y;
	void *pArg;


};

class Argument : public ValueObject {
public:
	static DynamicSegment *pDynamicSegment;
private:
	int a;
	int b;
};

#endif