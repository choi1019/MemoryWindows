#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"
#include <math.h>

class Page {
public:
};

class PageIndex : public SystemMemoryObject {
private:
	Page* m_pPage;
	size_t m_numPagesAllocated;
	size_t m_szPage;
	size_t m_index;

	bool m_bAllocated;

public:
	PageIndex(size_t pMemoryAllocated,  size_t szPage, size_t index)
		: m_pPage((Page*)pMemoryAllocated)
		, m_numPagesAllocated(1)
		, m_szPage(szPage)
		, m_index(index)
		, m_bAllocated(false)
	{
//		LOG_HEADER("PageIndex::PageINdex(pMemoryAllocated, numPages, szPage)", pMemoryAllocated, numPages, szPage);

//		LOG_FOOTER("PageIndex::PageINdex");
	}
	virtual ~PageIndex() {}

	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

	Page* GetPPage() { return this->m_pPage; }
	//	void SetPPage(Page* pPage) { this->m_pPage = pPage; }

	size_t GetNumPages() { return this->m_numPagesAllocated; }
	void SetNumPages(size_t numPagesAllocated) { this->m_numPagesAllocated = numPagesAllocated; }

	size_t GetIndex() { return this->m_index; }

	bool IsAllocated() { return this->m_bAllocated; }
	void SetIsAllocated(bool bAllocated) { this->m_bAllocated = bAllocated; }

	void MAlloc(size_t numPagesAllocated) {
		m_numPagesAllocated = numPagesAllocated;
	}
	size_t Free() {
		size_t numPagesAllocated = m_numPagesAllocated;
		m_numPagesAllocated = 1;
		return numPagesAllocated;
	}

	void Show(const char* pTitle) {
		LOG_NEWLINE("PageIndex::show(index, m_pPage, m_bAllocated)", m_index, (size_t)m_pPage, m_bAllocated);
	}
};
