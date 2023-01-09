#pragma once

#include "../Config.h"

#include "../../../Camera/PApplication/PMain/PMain.h"
#include "../../../Platform/5PPlatform/PMemoryManager/PMemoryVariable.h"
#include "../../../Platform/5PPlatform/PMemoryManager/PMemoryEven.h"
#include "../../../Platform/5PPlatform/PMemoryManager/PMemoryManager.h"
#include "../../../Platform/1Base/Aspect/Exception.h"

#include "../../../Platform/6PTechnical/PSocketClient/PSocketClient.h"

class TestMain
{
public:
	TestMain() {

	}
	~TestMain() {

	}

	void Run() {
		try {
			int szTotalMemory1 = sizeof(PMemoryManager) + sizeof(size_t);
			int szTotalMemory2 = sizeof(PMemoryVariable) + SIZE_MEMORYBASE + 2 * sizeof(size_t);
			int szTotalMemory3 = sizeof(PMemoryEven) + COUNT_EXCEPTION * sizeof(Exception) + 2 * sizeof(size_t);
			int szTotalMemory4 = sizeof(PMemoryEven) + COUNT_LOG * sizeof(Log) + 2 * sizeof(size_t);
			int szTotalMemory5 = sizeof(PMemoryEven) + COUNT_EVENT * sizeof(Event) + 2 * sizeof(size_t);
			int szTotalMemory = szTotalMemory1 + szTotalMemory2 + szTotalMemory3 + szTotalMemory4 + szTotalMemory5;

			char *m_pMemeoryAllocated = new char[szTotalMemory];
			Memory::s_pMemoryManager = new(m_pMemeoryAllocated) PMemoryManager(szTotalMemory);

			// reset static memory
			dynamic_cast<PMemoryManager*>(Memory::s_pMemoryManager)->InitializeMemory(
				SIZE_MEMORYBASE,
				COUNT_EXCEPTION,
				COUNT_LOG,
				COUNT_EVENT
			);
			/// /////////////////////////////////////////////////////////////////////////////////
			/// Application
			/// /////////////////////////////////////////////////////////////////////////////////
			PMain* pPMain = new("PMain") PMain();
			pPMain->AddParts(dynamic_cast<PMemoryManager*>(Memory::s_pMemoryManager));

			pPMain->BootstrapSystem();
			pPMain->Run();
			pPMain->CloseSystem();

			delete pPMain;

			/// /////////////////////////////////////////////////////////////////////////////////
			Memory::s_pMemoryManager->Show("Main::MemoryManager");
			Memory::s_pMemoryManager->FinalizeMemory();
			delete Memory::s_pMemoryManager;
			delete[] m_pMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
		}
	}
};

