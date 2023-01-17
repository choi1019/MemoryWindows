#pragma once

#include "../typedef.h"
#include "../../01Base/Memory/IMemory.h"

class Page {
public:
	Page* pNext;
};

class PageIndex : public IMemory {
private:
	Page* m_pPage;
	PageIndex* m_pNext;
	size_t m_numPages;
public:
	PageIndex(Page *pPage)
		: m_pPage(pPage)
		, m_pNext(nullptr)
		, m_numPages(0)
	{
	}
	virtual ~PageIndex() {}

	Page* GetPPage() { return this->m_pPage; }
	void SetPPage(Page* pPage) { this->m_pPage = pPage; }
	PageIndex* GetPNext() { return this->m_pNext; }
	void SetPNext(PageIndex* pNext) { this->m_pNext = pNext; }
	size_t GetNumPages() { return this->m_numPages; }
	void SetNumPages(size_t numPages) { this->m_numPages = numPages; }

	void Show(const char* pTitle) {
		size_t pNextPage = 0;
		if (m_pNext != nullptr) {
			pNextPage = (size_t)m_pNext->GetPPage();
		}
		LOG("PageIndex::show", (size_t)m_pPage, pNextPage);
	}

};

class PageManager : public IMemory {
private:
	void* m_pMemoryAllocated;
	size_t m_szMemoryAllocated;

	size_t m_numPages;
	size_t m_szPage;

	// pageIndex
	PageIndex* m_pHead;

public:
	PageManager(void* pMemeoryAllocated, size_t szMemoryAllocated, size_t szPage) {

		this->m_pMemoryAllocated = pMemeoryAllocated;

		this->m_szMemoryAllocated = szMemoryAllocated;
		if (this->m_szMemoryAllocated < m_szPage) {
			throw Exception(static_cast<unsigned>(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage));
		}
		this->m_szPage = szPage;
		this->m_numPages = szMemoryAllocated / szPage;

		for (int i = 0; i < m_numPages; i++) {
			// allocate pages in reverse order
			PageIndex* pPageIndex = new PageIndex((Page*)((size_t)(pMemeoryAllocated)+(m_szPage*(m_numPages -1 - i))));
			pPageIndex->SetPNext(this->m_pHead);
			this->m_pHead = pPageIndex;
		}

		LOG_HEADER("PageManager::PageManager", szMemoryAllocated, m_numPages, m_szPage);
		PageIndex* pPageIndex = this->m_pHead;
		while (pPageIndex != nullptr) {
			LOG("Page", (size_t)pPageIndex->GetPPage());
			pPageIndex = pPageIndex->GetPNext();
		}
		LOG_FOOTER("PageManager");
	}

	PageIndex* Malloc(size_t numPagesAllocated) {
		this->m_numPages -= numPagesAllocated;

		// a page before the target page
		PageIndex* pStartPageIndex = this->m_pHead;
		// the last target page
		PageIndex* pLastPageIndex = this->m_pHead;
		// look ahead pointer usually after the LastPageIndex
		PageIndex* pPageIndex = this->m_pHead;

		LOG_HEADER("PageManager::Malloc", numPagesAllocated, m_numPages);

		size_t count = 0;
		while (pLastPageIndex != nullptr) {
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
				LOG_FOOTER("PageManager::Malloc", 
					(size_t)pPageIndexAllocated->GetPPage(), pPageIndexAllocated->GetNumPages());
				return pPageIndexAllocated;
			}

			// at least two or more PageIndexs
			if (pPageIndex != m_pHead && pPageIndex != nullptr) {
				// if pages are not consecutive, reset count
				if ((size_t)(pLastPageIndex->GetPPage()) + m_szPage != (size_t)(pPageIndex->GetNumPages())) {
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
		throw Exception((unsigned)IMemory::EException::_eNoMorePage);
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

	size_t GetSzPage() { return this->m_szPage; }
	size_t GetNumPages() { return this->m_numPages; }

	void Show(const char* pTitle) {
		PageIndex* pCurrentPageIndex = m_pHead;
		PageIndex* pPreviousPageIndex = m_pHead;
		LOG_HEADER("PageManager::Show", m_szMemoryAllocated, m_szPage, m_numPages);
		while (pCurrentPageIndex != nullptr) {
			pCurrentPageIndex->Show("");
			pPreviousPageIndex = pCurrentPageIndex;
			pCurrentPageIndex = pCurrentPageIndex->GetPNext();
		}
		LOG_FOOTER("PageManager");
	}
};

