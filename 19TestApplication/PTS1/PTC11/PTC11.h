#pragma once

#include "../../typedef.h"
#define _PTC11_ID _GET_CLASS_UID(_ELayer_TestApplication::_ePTC11)
#define _PTC11_NAME "PTC11"

#include "../../../11TestPlatform/TestCase/TestCase.h"

#include "Config.h"
/*
#include "../../../../0Camera/PApplication/PMain/PMain.h"
#include "../../../../0Platform/5PPlatform/PMemoryManager/PMemoryVariable.h"
#include "../../../../0Platform/5PPlatform/PMemoryManager/PMemoryEven.h"
#include "../../../../0Platform/5PPlatform/PMemoryManager/PMemoryManager.h"
#include "../../../../0Platform/1Base/Aspect/Exception.h"
*/

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
		/*
		try {
			int szTotalMemory1 = sizeof(PMemoryManager) + sizeof(size_t);
			int szTotalMemory2 = sizeof(PMemoryVariable) + PTC11_SIZE_MEMORYBASE + 2 * sizeof(size_t);
			int szTotalMemory3 = sizeof(PMemoryEven) + PTC11_COUNT_EXCEPTION * sizeof(Exception) + 2 * sizeof(size_t);
			int szTotalMemory4 = sizeof(PMemoryEven) + PTC11_COUNT_LOG * sizeof(Log) + 2 * sizeof(size_t);
			int szTotalMemory5 = sizeof(PMemoryEven) + PTC11_COUNT_EVENT * sizeof(Event) + 2 * sizeof(size_t);
			int szTotalMemory = szTotalMemory1 + szTotalMemory2 + szTotalMemory3 + szTotalMemory4 + szTotalMemory5;

			this->m_pMemeoryAllocated = new char[szTotalMemory];
			Memory::s_pMemoryManager = new(m_pMemeoryAllocated) PMemoryManager(szTotalMemory);

			// reset static memory
			dynamic_cast<PMemoryManager *>(Memory::s_pMemoryManager)->InitializeMemory(
				PTC11_SIZE_MEMORYBASE,
				PTC11_COUNT_EXCEPTION,
				PTC11_COUNT_LOG,
				PTC11_COUNT_EVENT
			);

			PMain* pPMain = new("PMain") PMain();
			pPMain->AddParts(dynamic_cast<PMemoryManager *>(Memory::s_pMemoryManager));

			pPMain->BootstrapSystem();
			pPMain->Run();
			pPMain->CloseSystem();

			delete pPMain;

			Memory::s_pMemoryManager->Show("PTC11_Main::MemoryManager");
			Memory::s_pMemoryManager->FinalizeMemory();
			delete Memory::s_pMemoryManager;
			delete this->m_pMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
		*/
	}
};

