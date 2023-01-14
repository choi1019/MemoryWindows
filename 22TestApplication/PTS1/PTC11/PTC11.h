#pragma once

#include "Config.h"
#define _PTC11_ID _GET_TESTCASE_UID(_ELayer_PTS1::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../21TestPlatform/TestCase/TestCase.h"
#include "../../../01Base/Aspect/Exception.h"
#include "../../../13PTechnical/PMemoryManager/PMemory.h"
#include "../../../01Base/StdLib/Vector.h"

#define SIZE_ARRARY 10
class TestMemoryObject : public BaseObject {
private:
	int m_pArray[SIZE_ARRARY];
	Vector<int>* m_pVector;

public:
	TestMemoryObject() : BaseObject() {
		this->m_pVector = new("") Vector<int>();

		for (int i = 0; i < SIZE_ARRARY; i++) {
			this->m_pArray[i] = i;
		}
	}
	virtual ~TestMemoryObject() {
	}

	void Show(const char* pcTitle) {
		LOG_HEADER("TestMemoryObject::Show", String(sizeof(*this)));
		for (int i = 0; i < SIZE_ARRARY; i++) {
			LOG(this->m_pArray[i]);
		}
		LOG_FOOTER("TestMemoryObject");
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
			pMemory->Show("");

			TestMemoryObject* pTestMemoryObject = new("TestObject") TestMemoryObject();
			pTestMemoryObject->Show("TestObject");

			pMemory->Show("");

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

