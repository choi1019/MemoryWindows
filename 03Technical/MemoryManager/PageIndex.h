#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"
#include <math.h>

class Page {
public:
//	Page* pNext;
};

class PageIndex : public SystemMemoryObject {
private:
	size_t m_numPages;
	size_t m_szPage;

	size_t m_index;
	Page* m_pPage;
	PageIndex* m_pNext;
public:
	PageIndex(size_t numPages, size_t szPage)
		: m_numPages(numPages)
		, m_szPage(szPage)

		, m_index(0)
		, m_pPage(nullptr)
		, m_pNext(nullptr)
	{
		m_numPages--;
		LOG_NEWLINE("PageIndex::PageIndex-", m_numPages, m_szPage);
		if (m_numPages > 0) {
			// generate PageIndex
			m_pNext = new("") PageIndex(m_numPages, szPage);
		}
	}
	virtual ~PageIndex() {}

	virtual void Initialize(size_t pMemoryAllocated) {
		// allocate a m_pPage - User Memory
		m_pPage = (Page*)pMemoryAllocated;
		// index as a Page Address
		this->m_index = ((size_t)m_pPage >> (size_t)log2((double)m_szPage));

		LOG_NEWLINE("PageIndex::Initialize-", m_numPages, m_index, (size_t)m_pPage);
		if (m_pNext != nullptr) {
			m_pNext->Initialize((size_t)pMemoryAllocated + m_szPage);
		}
	}
	virtual void Finalize() {
	}

	size_t GetIndex() { return this->m_index; }
	Page* GetPPage() { return this->m_pPage; }
	void SetPPage(Page* pPage) { this->m_pPage = pPage; }
	PageIndex* GetPNext() { return this->m_pNext; }
	void SetPNext(PageIndex* pNext) { this->m_pNext = pNext; }
	size_t GetNumPages() { return this->m_numPages; }
	void SetNumPages(size_t numPages) { this->m_numPages = numPages; }

	void Show(const char* pTitle) {
		if (m_pNext != nullptr) {
			LOG_NEWLINE("PageIndex::show", (size_t)m_pPage, (size_t)m_pNext->GetPPage());
			m_pNext->Show("");
		}
		else {
			LOG_NEWLINE("PageIndex::show", (size_t)m_pPage, (size_t)m_pNext);
		}
	}

};
