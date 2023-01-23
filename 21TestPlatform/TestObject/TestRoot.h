#pragma once

#include "../typedef.h"
#define _TestRoot_id _GET_CLASS_UID(_ELayer_TestPlatform::_eTestRoot)
#define _TestRoot_Name "TestRoot"

#define MAX_TESTCASES 20

class TestRoot {
public:
	// static members
	void* operator new(size_t szThis) {
		printf("\n#new TestRoot(%zu)", szThis);
		return malloc(szThis);
	}
	void operator delete(void* pObject) {
		printf("\n#delete (%zu)", (size_t)pObject);
		free(pObject);
	}
	void operator delete(void* pObject, const size_t szSystemMemory, void* pSystemMemory) {
	}
private:
	// class variable
	static unsigned s_uCounter;

	// attributes
	int m_uObjectId;
	int m_nClassId;
	const char* m_pcClassName;
	size_t m_szThis;

public:
	TestRoot(
		int nClassId = _TestRoot_id,
		const char* pcClassName = _TestRoot_Name)
		: m_uObjectId(s_uCounter++)
		, m_nClassId(nClassId)
		, m_pcClassName(pcClassName)
		, m_szThis(0)
	{
	}
	virtual ~TestRoot() {}

	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void Run() {}

	// getters and setters
	inline int GetObjectId() { return this->m_uObjectId; }
	inline int GetClassId() { return this->m_nClassId; }
	inline const char* GetClassName() { return this->m_pcClassName; }

	size_t GetSize() { return this->m_szThis; }
	void SetSize(size_t szThis) { this->m_szThis = szThis; }
};
