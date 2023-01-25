#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"
#include "PageIndex.h"

class PageList : public SystemMemoryObject {
private:
//	void* m_pMemoryAllocated;
	size_t m_szMemoryAllocated;
	size_t m_szPage;

	size_t m_numPagesMax;
	size_t m_numPagesAvaiable;

	// pageIndex
	PageIndex** m_apPageIndices;

public:
	PageList(size_t pMemeoryAllocated, size_t szMemoryAllocated, size_t szPage) :
		m_szMemoryAllocated(szMemoryAllocated),
		m_szPage(szPage)
	{
		LOG_HEADER("PageList::PageList(pMemeoryAllocated,m_szMemoryAllocated,m_szPage)"
			, pMemeoryAllocated, m_szMemoryAllocated, m_szPage);

		if (m_szMemoryAllocated < m_szPage) {
			throw Exception((unsigned)(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage), "PageList", "PageList", "MemoryTooSmall");
		}
		this->m_numPagesMax = m_szMemoryAllocated / m_szPage;
		this->m_numPagesAvaiable = this->m_numPagesMax;

		// operator new[] for pointer array
//		this->m_apPageIndices = new("") PageIndex*[m_numPagesMax];
		this->m_apPageIndices = (PageIndex**)(SystemMemoryObject::operator new(sizeof(PageIndex*)*m_numPagesMax, "m_apPageIndices**"));
		for (int index = 0; index < this->m_numPagesMax; index++) {
			m_apPageIndices[index] = new((String("m_apPageIndices[]")+String()).c_str()) PageIndex(pMemeoryAllocated, m_szPage, index);
			pMemeoryAllocated += m_szPage;
		}
		LOG_FOOTER("PageList::PageList(m_numPages)", m_numPagesAvaiable);
	}
	virtual ~PageList() {
	}
	virtual void Initialize() {
		LOG_HEADER("PageList::Initialize");
		LOG_FOOTER("PageList::Initialize");
	}
	virtual void Finalize() {
	}

	size_t GetSzPage() { return this->m_szPage; }
	size_t GetNumPagesAvailable() { return this->m_numPagesAvaiable; }
	size_t GetNumPagesMax() { return this->m_numPagesMax; }

	PageIndex* Malloc(size_t numPagesRequired) {
		LOG_HEADER("PageList::Malloc(numPagesRequired, m_numPages)", numPagesRequired, m_numPagesAvaiable);
		size_t numPagesAllocated = numPagesRequired;
		unsigned index = 0;
		unsigned indexFound = 0;
		while (numPagesAllocated > 0) {
			if ((m_apPageIndices[index++]->IsAllocated())) {
				numPagesAllocated = numPagesRequired;
				indexFound = index;
			}
			else {
				numPagesAllocated--;
			}
			if (index > m_numPagesMax) {
				// not found
				throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
			}
		}

		m_apPageIndices[indexFound]->MAlloc(numPagesRequired);
		for (int i = 0; i < numPagesRequired; i++) {
			m_apPageIndices[i + indexFound]->SetIsAllocated(true);
		}
		m_numPagesAvaiable -= numPagesRequired;
		LOG_FOOTER("PageList::Malloc(indexFound)", indexFound, (size_t)(m_apPageIndices[indexFound]->GetPPage()));

		return m_apPageIndices[indexFound];
	}

	void Free(size_t indexFree) {
		LOG_HEADER("PageList::Free(indexFree)", indexFree);

		size_t numPagesAllocated = m_apPageIndices[indexFree]->Free();
		for (size_t i = 0; i < numPagesAllocated; i++) {
			m_apPageIndices[indexFree+i]->SetIsAllocated(false);
		}

		m_numPagesAvaiable += numPagesAllocated;
		LOG_FOOTER("PageList::Free(m_numPages)", m_numPagesAvaiable);
	}

	void Show(const char* pTitle) {
		LOG_HEADER("PageList::Show(m_szMemoryAllocated, m_szPage, m_numPages)"
			, m_szMemoryAllocated, m_szPage, m_numPagesAvaiable);
		for (int i=0; i< m_numPagesMax; i++) {
			m_apPageIndices[i]->Show("");
		}
		LOG_FOOTER("PageList");
	}
};

