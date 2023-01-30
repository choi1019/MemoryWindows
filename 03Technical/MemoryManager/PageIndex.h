#pragma once

#include <03Technical/typedef.h>
#define _PageIndex_Id _GET_CLASS_UID(_ELayer_Technical::_ePageIndex)
#define _PageIndex_Name "PageIndex"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <math.h>

class Page {
public:
};

class PageIndex : public MemoryObject {
private:
	Page* m_pPage;
	size_t m_szPage;
	size_t m_index;
	size_t m_numPagesAllocated;

	bool m_bAllocated;

public:
	Page* GetPPage() { return this->m_pPage; }
	//	void SetPPage(Page* pPage) { this->m_pPage = pPage; }
	size_t GetIndex() { return this->m_index; }
	size_t GetNumPages() { return this->m_numPagesAllocated; }
	void SetNumPages(size_t numPagesAllocated) { this->m_numPagesAllocated = numPagesAllocated; }
	bool IsAllocated() { return this->m_bAllocated; }
	void SetIsAllocated(bool bAllocated) { this->m_bAllocated = bAllocated; }

public:
	PageIndex(
		size_t pMemoryAllocated,  
		size_t szPage, 
		size_t index,
	 	int nClassId = _PageIndex_Id,
		const char* pClassName = _PageIndex_Name)
		: MemoryObject(nClassId, pClassName)
		
		, m_pPage((Page*)pMemoryAllocated)
		, m_szPage(szPage)
		, m_index(index)

		, m_numPagesAllocated(1)
		, m_bAllocated(false)
	{
	}
	virtual ~PageIndex() {}

	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

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
