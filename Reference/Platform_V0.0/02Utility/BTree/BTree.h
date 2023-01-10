//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef BTREE
#define BTREE "_BTree"
#include "../TypeDef.h"
#define BTREE_ID GETCLASSID(_eBTree_)

#include "BTreeNode.h"

// A _BTree 
// https://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
// https://www.geeksforgeeks.org/b-tree-set-1-insert-2/
// https://www.geeksforgeeks.org/b-tree-set-3delete/

template <class KEYTYPE, unsigned MAXLENGTH = 50>
class _BTree
{
public:
	typedef _BTreeNode<KEYTYPE, MAXLENGTH> Node;
	typedef _Pair<KEYTYPE, int> Pair;

private:
	// Pointer to pRoot node
	Node *pRoot;

public:
	// Constructor (Initializes tree as empty) 
	_BTree()
	{
		this->pRoot = NULL;
	}
	virtual ~_BTree() {}

	// function to traverse the tree 
	void traverse()
	{
		if (this->pRoot != NULL)
			this->pRoot->traverse();
	}
	// function to search a key in this tree 
	int search(KEYTYPE key)
	{
		if (this->pRoot == NULL) {
			return NOT_FOUND;
		}
		return this->pRoot->search(key);
	}
	// The main function that inserts a new key in this B-Tree 
	void insert(Pair element) throw()
	{
		// If tree is empty 
		if (this->pRoot == NULL)
		{
			// Allocate memory for pRoot 
			this->pRoot = new Node(element, true);
		}
		else // If tree is not empty 
		{
			// If pRoot is full, then tree grows in height 
			if (this->pRoot->isFullKey())
			{
				// Allocate memory for new pRoot 
				Node *pNewRoot = new Node(false);
				// Make old pRoot as child of new pRoot 
				pNewRoot->setPChild(0, pRoot);
				// Split the old pRoot and move 1 key to the new pRoot 
				pNewRoot->splitChild(0, pRoot);
				// pNewRoot has two children now.  
				// Decide which of the two children is going to have new key 
				int index = (pNewRoot->getElement(0).first >= element.first) ? 0 : 1;
				pNewRoot->getPChild(index)->insertNonFull(element);
				// Change pRoot 
				this->pRoot = pNewRoot;
			}
			else  // If pRoot is not full, call insertNonFull for pRoot 
				this->pRoot->insertNonFull(element);
		}
	}
};

/*
	static MemoryManager memoryManager;

	void *operator new (size_t size) throw() {
		return memoryManager.allocate(size);
	}
	void operator delete(void *p) throw() {
		memoryManager.free(p);
	}

};


template <class KEYTYPE, unsigned MAXLENGTH>
MemoryManager _BTree<KEYTYPE, MAXLENGTH>::memoryManager(
	sizeof(_BTree<KEYTYPE, MAXLENGTH>), sizeof(_BTree<KEYTYPE, MAXLENGTH>) * 10);

	*/

#endif