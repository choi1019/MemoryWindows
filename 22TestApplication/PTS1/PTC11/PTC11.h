#pragma once

#include "Config.h"
#define _PTC11_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../01Base/Aspect/Exception.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"

class TestMemoryObject : public BaseObject {
private:
	size_t m_szArray;
	int* m_pArray;
public:
	TestMemoryObject(size_t szArray) : BaseObject() {
		this->m_szArray = szArray;
		this->m_pArray = new int[szArray];
		for (int i = 0; i < this->m_szArray; i++) {
			this->m_pArray[i] = i;
		}
	}
	virtual ~TestMemoryObject() {
		delete this->m_pArray;
	}

	void Show(const char* pcTitle) {
		LOG_HEADER(pcTitle);
		for (int i = 0; i < this->m_szArray; i++) {
			LOG(this->m_pArray[i]);
		}
		LOG_FOOTER(pcTitle);
	}

};

class PTC11 : public TestCase {
private:
	char* m_pMemeoryAllocated;
public:
	PTC11(
		int nClassId = _PTC11_ID,
		const char* pcClassName = _PTC11_NAME)
		: TestCase(nClassId, pcClassName)
		, m_pMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTC11() {
	}

	void Initialize() {
		TestCase::Initialize();
	}
	void Finalize() {
		TestCase::Finalize();
	}

	void Run() {
		
		try {
			size_t szSystemMemory = 1024;
			char* pSystemMemoryAllocated = new char[szSystemMemory];
			IMemory::s_pSystemMemoryAllocated = pSystemMemoryAllocated;

			size_t szTotalMemory = 10000;
			this->m_pMemeoryAllocated = new char[szTotalMemory];

			Memory* pMemory = new PMemory(szTotalMemory, m_pMemeoryAllocated);
			BaseObject::s_pMemory = pMemory;

			pMemory->Show("Main");

			TestMemoryObject* pTestMemoryObject = new("TestObject") TestMemoryObject(10);
			pTestMemoryObject->Show("TestObject");

			delete pTestMemoryObject;
			delete pMemory;
			delete[] this->m_pMemeoryAllocated;
			delete[] pSystemMemoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
		
	}
};

