#pragma once

#include "../typedef.h"
#include "SystemMemoryObject.h"
#include "PageIndex.h"

class PageManager : public SystemMemoryObject {
private:
//	void* m_pMemoryAllocated;
	size_t m_szMemoryAllocated;
	size_t m_szPage;

	size_t m_numPages;

	// pageIndex
	PageIndex* m_pHead;

public:
	PageManager(size_t szMemoryAllocated, size_t szPage) {
		this->m_szMemoryAllocated = szMemoryAllocated;
		this->m_szPage = szPage;

		LOG_HEADER("PageManager::PageManager", m_szPage);
		if (m_szMemoryAllocated < m_szPage) {
			throw Exception(static_cast<unsigned>(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage));
		}
		this->m_numPages = m_szMemoryAllocated / m_szPage;
		this->m_pHead = new("") PageIndex(m_numPages, m_szPage);

		LOG_FOOTER("PageManager");
	}
	virtual ~PageManager() {

	}
	virtual void Initialize(void* pMemeoryAllocated) {
		LOG_HEADER("PageManager::Initialize", (size_t)pMemeoryAllocated);
		this->m_pHead->Initialize((size_t)pMemeoryAllocated);
		LOG_FOOTER("PageManager::Initialize");
	}
	virtual void Finalize() {
	}

	PageIndex* Malloc(size_t numPagesAllocated) {
		this->m_numPages -= numPagesAllocated;

		// a page before the target page
		PageIndex* pStartPageIndex = this->m_pHead;
		// the last target page
		PageIndex* pLastPageIndex = this->m_pHead;
		// look ahead pointer usually after the LastPageIndex
		PageIndex* pPageIndex = this->m_pHead;

		LOG_HEADER("PageManager::Malloc(numPagesAllocated, m_numPages)", numPagesAllocated, m_numPages);

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
				LOG_FOOTER("PageManager::Malloc(", 
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
		LOG_HEADER("PageManager::Show", m_szMemoryAllocated, m_szPage, m_numPages);
		m_pHead->Show("");
		LOG_FOOTER("PageManager");
	}
};

