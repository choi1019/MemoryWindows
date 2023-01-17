#pragma once

#include "../../01Base/Memory/IMemory.h"

class Page {
public:
	Page* pNext;
};

class PageIndex : public IMemory {
private:
	Page* m_pPage;
	PageIndex* m_pNext;
public:
	PageIndex(Page *pPage)
		: m_pPage(pPage)
		, m_pNext(nullptr)
	{
	}
	virtual ~PageIndex() {}

	Page* GetPPage() { return this->m_pPage; }
	void SetPPage(Page* pPage) { this->m_pPage = pPage; }
	PageIndex* GetPNext() { return this->m_pNext; }
	void SetPNext(PageIndex* pNext) { this->m_pNext = pNext; }

	void Show(const char* pTitle) {
		LOG_HEADER("PageIndex::show - Free", String((size_t)m_pPage), String((size_t)m_pNext));
		LOG_FOOTER("PageIndex");
	}

};

class PageManager : public IMemory {
private:
	size_t m_szPage;
	size_t m_numPages;
	size_t m_szMemoryAllocated;
	void* m_pMemoryAllocated;

	// pageIndex
	PageIndex* m_pHead;

public:
	PageManager(size_t szPage, size_t szMemoryAllocated, void *pMemeoryAllocated) {
		this->m_szPage = szPage;
		this->m_numPages = szMemoryAllocated / szPage;

		this->m_szMemoryAllocated = szMemoryAllocated;
		if (this->m_szMemoryAllocated < m_szPage) {
			throw Exception(static_cast<unsigned>(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage));
		}


		this->m_pMemoryAllocated = pMemeoryAllocated;
		for (int i = 0; i < m_numPages; i++) {
			PageIndex* pPageIndex = new PageIndex((Page*)((size_t)(pMemeoryAllocated)+(m_szPage*(m_numPages -1 - i))));
			pPageIndex->SetPNext(this->m_pHead);
			this->m_pHead = pPageIndex;
		}


		LOG_HEADER("PageManager::PageManager", m_numPages, m_szPage, szMemoryAllocated);
		PageIndex* pPageIndex = this->m_pHead;
		while (pPageIndex != nullptr) {
			LOG("Page", (size_t)pPageIndex->GetPPage());
			pPageIndex = pPageIndex->GetPNext();
		}
		LOG_FOOTER("PageManager");
	}

	PageIndex* Malloc(size_t numPagesAllocated) {
			// find available pages
		m_numPages -= numPagesAllocated;

		// start page
		PageIndex* pStartPageIndex = this->m_pHead;
		PageIndex* pPageIndex = this->m_pHead;
		PageIndex* pLastPageIndex = this->m_pHead;

		LOG_HEADER("PageManager::Malloc", numPagesAllocated, m_numPages);

		size_t count = 0;
		while (pLastPageIndex != nullptr) {
			if (count == numPagesAllocated) {
				PageIndex* pPageIndexAllocated = nullptr;
				if (pStartPageIndex == m_pHead) {
					pPageIndexAllocated = this->m_pHead;
					this->m_pHead = pLastPageIndex->GetPNext();
				} else {
					pPageIndexAllocated = pStartPageIndex->GetPNext();
					pStartPageIndex->SetPNext(pLastPageIndex->GetPNext());
				}
				pLastPageIndex->SetPNext(nullptr);
				LOG_FOOTER("PageManager::Malloc", (size_t)pPageIndexAllocated);
				return pPageIndexAllocated;
			}

			// at least two or more PageIndexs
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
		throw Exception();
	}

	void Free(PageIndex* pPageIndexFree) {
		if (m_pHead == nullptr) {
			this->m_pHead = pPageIndexFree;
		}
		// find a position to free pages
		PageIndex* pPageIndex = m_pHead;
		PageIndex* pPositionToFree = pPageIndex;
		while (pPageIndex != nullptr) {
			if (pPageIndex->GetPPage() > pPageIndexFree->GetPPage()) {
				break;
			}
			pPositionToFree = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
		}

		// find a last PageIndex from "pPageIndexFree"
		pPageIndex = pPageIndexFree;
		PageIndex* pLastPageIndex = pPageIndex;
		while (pPageIndex != nullptr) {
			pLastPageIndex = pPageIndex;
			pPageIndex = pPageIndex->GetPNext();
		}

		// insert "pPageIndexFree" to "pPositionToFree"
		pPositionToFree->SetPNext(pPageIndexFree);
		pLastPageIndex->SetPNext(pPositionToFree->GetPNext());
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

