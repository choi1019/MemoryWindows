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
	PageIndex* m_pPageIndexFirst;
	PageIndex* m_pPageIndexLast;

public:
	PageIndex(size_t pMemoryAllocated, size_t numPagesRequired, size_t index, size_t szPage)
		: m_szPage(szPage)
		, m_index(index)
		, m_numPages(1)
		, m_pNext(nullptr)
		, m_pPageIndexFirst(nullptr)
		, m_pPageIndexLast(nullptr)
	{
//		LOG_HEADER("PageIndex::PageINdex(pMemoryAllocated, numPages, szPage)", pMemoryAllocated, numPages, szPage);
		// allocate a m_pPage - User Memory
		m_pPage = (Page*)pMemoryAllocated + (m_index * m_szPage);
		if (numPagesRequired > 1) {
//			LOG_NEWLINE("if (numPages > 0)", numPages);
			// generate PageIndex
			m_pNext = new("") PageIndex(pMemoryAllocated, numPagesRequired-1, index+1, szPage);
		}
//		LOG_FOOTER("PageIndex::PageINdex");
	}
	virtual ~PageIndex() {}

	virtual void Initialize() {
	}
	virtual void Finalize() {
	}

	size_t GetIndex() { return this->m_index; }
	Page* GetPPage() { return this->m_pPage; }
//	void SetPPage(Page* pPage) { this->m_pPage = pPage; }
	PageIndex* GetPNext() { return this->m_pNext; }
	void SetPNext(PageIndex* pNext) { this->m_pNext = pNext; }

	size_t GetNumPages() { return this->m_numPages; }
	void SetNumPages(size_t numPages) { this->m_numPages = numPages; }

	PageIndex* GetPPageIndexFirst() { return this->m_pPageIndexFirst; }
	PageIndex* GetPPageIndexLast() { return this->m_pPageIndexLast; }

	void SetPPageIndexLast(PageIndex* pPageIndexFirst) { this->m_pPageIndexLast = pPageIndexFirst; }

	PageIndex* Malloc(size_t numPagesCurrent, size_t numPagesRequired, PageIndex *pPageIndexFirst) {
		LOG_HEADER("PageIndex::Malloc(numPagesCurrent, numPagesRequired, pPageIndexFirst)"
			, numPagesCurrent, numPagesRequired, (size_t)pPageIndexFirst);
		this->m_numPages = numPagesCurrent;
		numPagesCurrent--;
		if (numPagesCurrent == 0) {
			LOG_NEWLINE("if (numPagesCurrent == 0)", numPagesCurrent);
			// found last PageIndex
			this->m_pPageIndexFirst = pPageIndexFirst;
			LOG_FOOTER("PageIndex::Malloc1");
			return this;
		} else if (numPagesCurrent > 0) {
			LOG_NEWLINE("if (numPagesCurrent > 0)", numPagesCurrent);
			if (this->GetPNext() != nullptr) {
				LOG_NEWLINE("if (this->GetPNext() != nullptr)");
				PageIndex* pPageIndexLast = nullptr;
				if (this->GetIndex() + 1 == this->GetPNext()->GetIndex()) {
					LOG_NEWLINE("if (this->GetIndex() + 1 == this->GetPNext()->GetIndex())");
					// if next page is continuous
					pPageIndexLast = this->GetPNext()->Malloc(numPagesCurrent, numPagesRequired, pPageIndexFirst);
				}
				else {
					LOG_NEWLINE("else (this->GetIndex() + 1 != this->GetPNext()->GetIndex())");
					// if discontinued, reset next as the first page
					// this is the page, prior to the real first
					pPageIndexFirst = this->GetPNext();
					pPageIndexLast = this->GetPNext()->Malloc(numPagesRequired, numPagesRequired, pPageIndexFirst);
					if (pPageIndexLast != nullptr) {
						LOG_NEWLINE("if (pPageIndexLast != nullptr)");
						pPageIndexFirst->SetPPageIndexLast(pPageIndexLast);
						this->SetPNext(pPageIndexLast->GetPNext());
						pPageIndexLast->SetPNext(nullptr);
					}
				}
				LOG_FOOTER("PageIndex::Malloc2");
				return pPageIndexLast;
			}
		}
		throw Exception((unsigned)IMemory::EException::_eNoMorePage, "PageIndex", "Malloc", "NoMoreConsecutivePage");
	}

	PageIndex* Free(PageIndex* pPageIndexFree, PageIndex* pPrevious) {
		LOG_NEWLINE("PageIndex::Free(pPageIndexFree, pPrevious)", (size_t)pPageIndexFree, (size_t)pPrevious);
		if (pPageIndexFree->GetIndex() > this->GetIndex()) {
			LOG_NEWLINE("if (pPageIndexFree->GetIndex() > this->GetIndex())"
				, (size_t)pPageIndexFree->GetIndex(), (size_t)this->GetIndex());
			if (this->GetPNext() != nullptr) {
				LOG_NEWLINE("if (this->GetPNext() != nullptr)", (size_t)(this->GetPNext()));
				this->GetPNext()->Free(pPageIndexFree, this);
			} else {
				LOG_NEWLINE("if (this->GetPNext() == nullptr)", (size_t)(this->GetPNext()));
				// if this is a last node
				this->SetPNext(pPageIndexFree);
			}
//			LOG_FOOTER("PageIndex::Free1");
			return this;
		} else {
			LOG_NEWLINE("if (pPageIndexFree->GetIndex() <= this->GetIndex())"
				, (size_t)pPageIndexFree->GetIndex(), (size_t)this->GetIndex());
			if (pPrevious == this) {
				// if this is a head node
				LOG_NEWLINE("if (pPrevious == this)", (size_t)pPrevious, (size_t)this);
			} else {
				LOG_NEWLINE("if (pPrevious != this)", (size_t)pPrevious, (size_t)this);
				// insert at the middle
				pPrevious->SetPNext(pPageIndexFree);
			}
			pPageIndexFree->GetPPageIndexLast()->SetPNext(this);
//			LOG_FOOTER("PageIndex::Free2");
			return pPageIndexFree;
		}
	}

	void Show(const char* pTitle) {
		if (m_pNext != nullptr) {
			LOG_NEWLINE("PageIndex::show(index, m_pPage)", m_index, (size_t)m_pPage);
			m_pNext->Show("");
		}
		else {
			LOG_NEWLINE("PageIndex::show(index, m_pPage)", m_index, (size_t)m_pPage);
		}
	}

};
