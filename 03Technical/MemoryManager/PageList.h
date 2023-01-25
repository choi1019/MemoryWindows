#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"
#include "PageIndex.h"

class PageList : public SystemMemoryObject {
private:
//	void* m_pMemoryAllocated;
	size_t m_szMemoryAllocated;
	size_t m_szPage;

	size_t m_numPages;

	// pageIndex
	PageIndex* m_pHead;

public:
	PageList(size_t pMemeoryAllocated, size_t szMemoryAllocated, size_t szPage) {
		this->m_szMemoryAllocated = szMemoryAllocated;
		this->m_szPage = szPage;

		LOG_HEADER("PageList::PageList(pMemeoryAllocated,m_szMemoryAllocated,m_szPage)"
			, pMemeoryAllocated, m_szMemoryAllocated, m_szPage);

		if (m_szMemoryAllocated < m_szPage) {
			throw Exception((unsigned)(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage), "PageList", "PageList", "MemoryTooSmall");
		}
		this->m_numPages = m_szMemoryAllocated / m_szPage;
		this->m_pHead = new("PageList-PageIndex") PageIndex(pMemeoryAllocated, m_numPages, 0, m_szPage);

		LOG_FOOTER("PageList::PageList");
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
	size_t GetNumPages() { return this->m_numPages; }

	PageIndex* Malloc(size_t numPagesRequired) {
		LOG_HEADER("PageList::Malloc(numPagesRequired, m_numPages)", numPagesRequired, m_numPages);
		if (this->m_pHead != nullptr) {
			// if pages are available
			LOG_NEWLINE("if (this->m_pHead != nullptr)");
			// allocate a page
			PageIndex *pPageIndexLast = this->m_pHead->Malloc(numPagesRequired, numPagesRequired, this->m_pHead);
			if (pPageIndexLast != nullptr) {
				LOG_NEWLINE("if (pPageIndexLast != nullptr)");
				// get the first page
				PageIndex* pPageIndexFirst = pPageIndexLast->GetPPageIndexFirst();
				if (pPageIndexFirst == m_pHead) {
					LOG_NEWLINE("if (pPageIndexFirst == m_pHead)");
					this->m_pHead = pPageIndexLast->GetPNext();

					pPageIndexFirst->SetPPageIndexLast(pPageIndexLast);
					pPageIndexLast->SetPNext(nullptr);
				}
				m_numPages--;
				LOG_FOOTER("PageList::Malloc(pPage)", (size_t)pPageIndexFirst->GetPPage());
				return pPageIndexFirst;
			}
		}
		throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
	}
	void Free(PageIndex* pPageIndexFree) {
		m_numPages++;
		LOG_HEADER("PageList::Free(pPageIndexFree)", (size_t)pPageIndexFree);
		if (this->m_pHead == nullptr) {
			LOG_NEWLINE("if (this->m_pHead == nullptr)");
			this->m_pHead = pPageIndexFree;
		}
		else {
			this->m_pHead = this->m_pHead->Free(pPageIndexFree, m_pHead);
		}
		LOG_FOOTER("PageList::Free(m_numPages)", m_numPages);
	}
/*
	PageIndex* Malloc(size_t numPagesAllocated) {
		this->m_numPages -= numPagesAllocated;

		// a page before the target page
		PageIndex* pStartPageIndex = this->m_pHead;
		// the last target page
		PageIndex* pLastPageIndex = this->m_pHead;
		// look ahead pointer usually after the LastPageIndex
		PageIndex* pPageIndex = this->m_pHead;

		LOG_HEADER("PageList::Malloc(numPagesAllocated, m_numPages)", numPagesAllocated, m_numPages);

		size_t count = 0;
		while (pPageIndex != nullptr) {
			// if found
			if (count == numPagesAllocated) {
				PageIndex* pPageIndexAllocated = nullptr;
				// if the target pages at the m_pHead
				if (pStartPageIndex == m_pHead) {
					pPageIndexAllocated = this->m_pHead;
					this->m_pHead = pLastPageIndex->GetPNext();
				// if the target pages in the middle
				} else {
					pPageIndexAllocated = pStartPageIndex->GetPNext();
					pStartPageIndex->SetPNext(pLastPageIndex->GetPNext());
				}
				pLastPageIndex->SetPNext(nullptr);
				pPageIndexAllocated->SetNumPages(numPagesAllocated);
				LOG_FOOTER("PageList::Malloc(pPage)", (size_t)pPageIndexAllocated->GetPPage());
				return pPageIndexAllocated;
			}

			// two or more Pages
			if (pPageIndex != m_pHead && pPageIndex != nullptr) {
				// if pages are not consecutive, reset count
				if ((size_t)(pLastPageIndex->GetPPage()) + m_szPage != (size_t)(pPageIndex->GetPPage())) {
					count = 0;
					PageIndex* pStartPageIndex = pLastPageIndex;
				}
			}

			// prceed to next PageIndex
			pLastPageIndex = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
			count++;
		}

		// allocated Pages
		throw Exception((unsigned)IMemory::EException::_eNoMorePage, "PageIndex", "Malloc","_eNoMorePage");
	}

	void Free(PageIndex* pPageIndexFree) {
		if (m_pHead == nullptr) {
			this->m_pHead = pPageIndexFree;
			return;
		}
		// find a last PageIndex from "pPageIndexFree"
		PageIndex* pPageIndex = pPageIndexFree;
		PageIndex* pLastPageIndex = pPageIndex;
		while (pPageIndex != nullptr) {
			pLastPageIndex = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
		}

		// find a position to free pages
		if (m_pHead->GetPPage() > pPageIndexFree->GetPPage()) {
			// insert "pPageIndexFree" to "pPositionToFree"
			pLastPageIndex->SetPNext(m_pHead);
			m_pHead = pPageIndexFree;
		}
		else {
			pPageIndex = m_pHead;
			PageIndex* pPositionToFree = pPageIndex;
			while (pPageIndex != nullptr) {
				if (pPageIndex->GetPPage() > pPageIndexFree->GetPPage()) {
					break;
				}
				pPositionToFree = pPageIndex;
				pPageIndex = pPageIndex->GetPNext();
			}
			// insert "pPageIndexFree" to "pPositionToFree"
			pLastPageIndex->SetPNext(pPositionToFree->GetPNext());
			pPositionToFree->SetPNext(pPageIndexFree);
		}
		this->m_numPages += pPageIndexFree->GetNumPages();
	}
*/
	void Show(const char* pTitle) {
		LOG_HEADER("PageList::Show(m_szMemoryAllocated, m_szPage, m_numPages)"
			, m_szMemoryAllocated, m_szPage, m_numPages);
		m_pHead->Show("");
		LOG_FOOTER("PageList");
	}
};

