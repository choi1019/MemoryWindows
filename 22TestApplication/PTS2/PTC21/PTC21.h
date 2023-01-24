#pragma once

#include "Config.h"
#define _PTC21_ID _GET_TESTCASE_UID(_ELayer_PTS2::_ePTC21)
#define _PTC21_NAME "PTC21"

#include "../../../21TestPlatform/TestCase/TestCase.h"

class TestPage {
private:
	int m_nSlots[10];
public:
	TestPage(unsigned index) {
		for (int i = 0; i < 10; i++) {
			m_nSlots[i] = index * 100 + i;
		}
	}
};

class TestPageIndex {
private:
	unsigned m_index;
	TestPage* m_pPage;
public:
	TestPageIndex(unsigned index) :
		m_index(index)
	{
		m_pPage = new TestPage(index);
	}
	TestPage* GetPPage() { return this->m_pPage; }
	void SetPPage(TestPage* pPage) { this->m_pPage = pPage; }
};

class TestMemory {
private:
	void* m_pMemoryAllocated;
	size_t m_szPage;
public:
	TestMemory() {
		m_pMemoryAllocated = nullptr;
		m_szPage = 0;
	}
	~TestMemory() {
	}
	void* Malloc(size_t szObject) {
	}
	void Free(void* pObject) {
	}
	void* GetPAddress(void* pObject) {
		size_t index = ((size_t)pObject - (size_t)m_pMemoryAllocated) / m_szPage;
		if ((index * m_szPage + (size_t)m_pMemoryAllocated) < (size_t)pObject) {
			index++;
		}
	}
};

class TestBaseObject {
public:
	static TestMemory* s_pMemory;
private:
	size_t m_index;
public:
	void* operator new(size_t szThis) {
		//		printf("\n\n@BaseObject::new %s (%zu)", sMessage, szThis);
		s_pMemory->Malloc(szThis);
	}
	void operator delete(void* pObject) {
		//		printf("\n@BaseObject::delete %zu\n", (size_t)pObject);
		s_pMemory->Free(pObject);
	}
	void* operator->() {
		void* pAddress = s_pMemory->GetPAddress(this);
		return pAddress;
	}
public:
	TestBaseObject()
	{
		m_index = 0;
	}


};

class PTC21 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC21(
		int nClassId = _PTC21_ID,
		const char* pcClassName = _PTC21_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC21() {
	}

	void Initialize() {
	}
	void Finalize() {
	}

	void Run() {
	}
};

