//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef BTREENODE
#define BTREENODE "_BTreeNode"
#include "../TypeDef.h"
#define BTREENODE_ID GETCLASSID(_eBTreeNode_)

#include "../../01Global/Object/ValueObject.h"
#include "../../02Utility/Map/Pair.h"

#define MINIMUMDEGREE 3
#define MAXIMUMNUMKEYS MINIMUMDEGREE*2-1

// A BTree node 
template <class KEYTYPE, unsigned MAXLENGTH>
class _BTreeNode: public ValueObject
{
public:
	typedef _Pair<KEYTYPE, int> Pair;
private:
	int numElements;							// Current number of aryPair 
	Pair aryPair[MAXIMUMNUMKEYS];				// An array of aryPair 
	_BTreeNode *aryPChild[MAXIMUMNUMKEYS + 1];	// An array of pChild pointers
	bool bLeaf;									// Is true when node is bLeaf. Otherwise false 

public:
	// getters & setters
	inline bool isFullKey() { return (this->numElements == MAXIMUMNUMKEYS); }
	inline bool isEmptyKey() { return (this->numElements == 0); }
	inline bool isLeaf() { return this->bLeaf; }

	inline Pair getElement(int index) { return this->aryPair[index]; }

	inline _BTreeNode *getPChild(int index) { return this->aryPChild[index]; }
	inline void setPChild(int index, _BTreeNode* pNode) { this->aryPChild[index] = pNode; }

	_BTreeNode(bool bLeaf) : ValueObject(Make_ClassInfo(BTREENODE_ID, BTREENODE))
	{
		this->numElements = 0;
		this->bLeaf = bLeaf;
	}

	_BTreeNode(Pair element, bool bLeaf) : ValueObject(Make_ClassInfo(BTREENODE_ID, BTREENODE))
	{
		this->numElements = 1;
		this->aryPair[0] = element;
		this->bLeaf = bLeaf;
	}

	virtual ~_BTreeNode() {
	}

	// A utility function to split the pChild pNode of this node. 
	// index is index of pNode in pChild array aryPChild[].  
	// The Child pNode must be full when this function is called 
	void splitChild(int index, _BTreeNode *pNode) throw()
	{
		// Create a new node which is going to store (minimumDegree-1) aryPair of pNode 
		_BTreeNode *pNewNode = new _BTreeNode(pNode->bLeaf);
		pNewNode->numElements = MINIMUMDEGREE - 1;
		// Copy the last (minimumDegree-1) aryPair of pNode to pNewNode 
		for (int j = 0; j < MINIMUMDEGREE - 1; j++) {
			pNewNode->aryPair[j] = pNode->aryPair[j + MINIMUMDEGREE];
		}
		// Copy the last minimumDegree children of pNode to pNewNode 
		if (!(pNode->bLeaf)) {
			for (int j = 0; j < MINIMUMDEGREE; j++) {
				pNewNode->aryPChild[j] = pNode->aryPChild[j + MINIMUMDEGREE];
			}
		}
		// Reduce the number of aryPair in pNode 
		pNode->numElements = MINIMUMDEGREE - 1;

		// Since this node is going to have a new pChild, 
		// create space of new pChild 
		for (int j = this->numElements; j >= index + 1; j--) {
			this->aryPChild[j + 1] = this->aryPChild[j];
		}
		// Link the new pChild to this node 
		this->aryPChild[index + 1] = pNewNode;

		// A key of pNode will move to this node. 
		// Find location of new key and move all greater aryPair one space ahead 
		for (int j = this->numElements - 1; j >= index; j--) {
			this->aryPair[j + 1] = this->aryPair[j];
		}
		// Copy the middle key of pNode to this node 
		this->aryPair[index] = pNode->aryPair[MINIMUMDEGREE - 1];
		// Increment count of aryPair in this node 
		this->numElements = this->numElements + 1;
	}

	// A utility function to insert a new key in this node 
	// The assumption is, the node must be non-full when this 
	// function is called 
	void insertNonFull(Pair element) throw()
	{
		// Initialize index as index of rightmost element 
		int index = this->numElements - 1;

		// If this is a bLeaf node 
		if (this->bLeaf == true)
		{
			// a) Finds the location of new key to be inserted 
			// b) Moves all greater aryPair to one place ahead 
			for (; index >= 0 && this->aryPair[index].first > element.first; index--)
			{
				this->aryPair[index + 1] = this->aryPair[index];
			}

			// Insert the new key at found location
			this->aryPair[index + 1] = element;
			this->numElements++;
		}
		else // If this node is not bLeaf 
		{
			// Find the pChild which is going to have the new key 
			for (; index >= 0 && this->aryPair[index].first > element.first; index--) {
			}

			// See if the found pChild is full 
			if (this->aryPChild[index + 1]->isFullKey())
			{
				// If the pChild is full, then split it 
				splitChild(index + 1, this->aryPChild[index + 1]);

				// After split, the middle key of aryPChild[index] goes up and 
				// aryPChild[index] is splitted into two.  See which of the two 
				// is going to have the new key 
				if (this->aryPair[index + 1].first < element.first) {
					index++;
				}
			}
			this->aryPChild[index + 1]->insertNonFull(element);
		}
	}

	// A function to traverse all nodes in a subtree rooted with this node 
	void traverse()
	{
		// There are numElements aryPair and numElements+1 children, traverse through numElements aryPair 
		// and first numElements children 
		int index = 0;
		for (; index < numElements; index++) {
			// If this is not bLeaf, then before printing key[index], 
			// traverse the subtree rooted with pChild aryPChild[index]. 
			if (this->bLeaf == false) {
				this->aryPChild[index]->traverse();
			}
			std::cout << " " << aryPair[index].first << ", " << aryPair[index].second << std::endl;
		}
		// Print the subtree rooted with last pChild 
		if (this->bLeaf == false) {
			this->aryPChild[index]->traverse();
		}
	}

	// A function to search a key in subtree rooted with this node.
	int search(KEYTYPE key)
	{
		// Find the first key greater than or equal to key 
		int index = 0;
		for (; index < numElements; index++) {
			if (aryPair[index].first >= key) {
				break;
			}
		}
		// If the found key is equal to key, return this node 
		if (aryPair[index].first == key) {
			return aryPair[index].second;
		}
		// If key is not found here and this is a bLeaf node 
		if (bLeaf == true) {
			return NOT_FOUND;
		}
		// Go to the appropriate pChild 
		return aryPChild[index]->search(key);
	}
};

#endif