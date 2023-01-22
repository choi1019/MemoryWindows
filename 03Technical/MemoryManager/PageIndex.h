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
	PageIndex(size_t pMemoryAllocated, size_t numPages, size_t szPage)
		: m_szPage(szPage)
		, m_pPage(nullptr)
		, m_index(0)

		, m_numPages(1)
		, m_pNext(nullptr)
		, m_pPageIndexFirst(nullptr)
		, m_pPageIndexLast(nullptr)
	{
		// allocate a m_pPage - User Memory
		m_pPage = (Page*)pMemoryAllocated;
		// index as a Page Address
		this->m_index = ((size_t)m_pPage >> (size_t)log2((double)m_szPage));
//		LOG_NEWLINE("PageIndex::PageIndex(m_pPage,numPages,m_szPage)", (size_t)m_pPage, numPages, m_szPage);

		numPages--;
		if (numPages > 0) {
			// generate PageIndex
			m_pNext = new("") PageIndex(pMemoryAllocated + szPage, numPages, szPage);
		}
	}
	virtual ~PageIndex() {}

	virtual void Initialize() {
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
	PageIndex* GetPPageIndexFirst() { return this->m_pPageIndexFirst; }
	PageIndex* GetPPageIndexLast() { return this->m_pPageIndexLast; }
	void SetPPageIndexLast(PageIndex* pPageIndexFirst) { this->m_pPageIndexLast = pPageIndexFirst; }

	PageIndex* Malloc(size_t numPagesCurrent, size_t numPagesRequired, PageIndex *pPageIndexFirst) {
		this->m_numPages = numPagesCurrent;
		numPagesCurrent--;
		if (numPagesCurrent == 0) {
			// found last PageIndex
			this->m_pPageIndexFirst = pPageIndexFirst;
			return this;
		} else if (numPagesCurrent > 0) {
			if (this->GetPNext() != nullptr) {
				PageIndex* pPageIndexLast = nullptr;
				if (this->GetIndex() + 1 == this->GetPNext()->GetIndex()) {
					// if next page is continuous
					pPageIndexLast = this->GetPNext()->Malloc(numPagesCurrent, numPagesRequired, pPageIndexFirst);
				}
				else {
					// if discontinued, reset next as the first page
					// this is the page, prior to the real first
					pPageIndexFirst = this->GetPNext();
					pPageIndexLast = this->GetPNext()->Malloc(numPagesRequired, numPagesRequired, pPageIndexFirst);
					if (pPageIndexLast != nullptr) {
						pPageIndexFirst->SetPPageIndexLast(pPageIndexLast);
						this->SetPNext(pPageIndexLast->GetPNext());
						pPageIndexLast->SetPNext(nullptr);
					}
				}
				return pPageIndexLast;
			}
		}
		return nullptr;
	}

	PageIndex* Free(PageIndex* pPageIndexFree, PageIndex* pPrevious) {
		if (pPageIndexFree->GetIndex() < this->GetIndex()) {
			if (this->GetPNext() != nullptr) {
				this->GetPNext()->Free(pPageIndexFree, this);
			} else {
				// if this is a last node
				this->SetPNext(pPageIndexFree);
			}
			return this;
		} else {
			if (pPrevious == this) {
				// if this is a head node
			} else {
				// insert at the middle
				pPrevious->SetPNext(pPageIndexFree);
			}
			pPageIndexFree->GetPPageIndexLast()->SetPNext(this);
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
