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
	PageIndex** m_apPageIndeces;

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

		// this->m_pHead = new("PageList-PageIndex") PageIndex(pMemeoryAllocated, m_numPages, 0, m_szPage);
		this->m_apPageIndeces = new PageIndex*[m_numPagesAvaiable];
		for (int index = 0; index < this->m_numPagesAvaiable; index++) {
			m_apPageIndeces[index] = new("") PageIndex(pMemeoryAllocated, m_szPage, index);
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
	size_t GetNumMaxPages() { return this->m_numPagesAvaiable; }

	PageIndex* Malloc(size_t numPagesRequired) {
		LOG_HEADER("PageList::Malloc(numPagesRequired, m_numPages)", numPagesRequired, m_numPagesAvaiable);
		size_t numPagesAllocated = numPagesRequired;
		unsigned index = 0;
		unsigned indexFound = 0;
		while (numPagesAllocated > 0) {
			if ((m_apPageIndeces[index++]->IsAllocated())) {
				numPagesAllocated = numPagesRequired;
				indexFound = index;
			}
			else {
				numPagesAllocated--;
			}
			if (index == m_numPagesAvaiable) {
				// not found
				throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
			}
		}

		m_apPageIndeces[indexFound]->SetNumPages(numPagesRequired);
		for (int i = 0; i < numPagesRequired; i++) {
			m_apPageIndeces[i + indexFound]->SetIsAllocated(true);
		}
		m_numPagesAvaiable -= numPagesRequired;
		LOG_FOOTER("PageList::Malloc(indexFound)", indexFound);

		return m_apPageIndeces[indexFound];
	}

	void Free(unsigned indexFree) {
		LOG_HEADER("PageList::Free(indexFree)", indexFree);

		unsigned numPagesAllocated = m_apPageIndeces[indexFree]->GetNumPages();
		for (int i = 0; i < numPagesAllocated; i++) {
			m_apPageIndeces[i]->SetIsAllocated(false);
		}

		m_numPagesAvaiable += numPagesAllocated;
		LOG_FOOTER("PageList::Free(m_numPages)", m_numPagesAvaiable);
	}

	void Show(const char* pTitle) {
		LOG_HEADER("PageList::Show(m_szMemoryAllocated, m_szPage, m_numPages)"
			, m_szMemoryAllocated, m_szPage, m_numPagesAvaiable);
		for (int i=0; i< m_numPagesAvaiable; i++) {
			m_apPageIndeces[i]->Show("");
		}
		LOG_FOOTER("PageList");
	}
};

