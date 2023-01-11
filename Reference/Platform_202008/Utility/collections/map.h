#pragma once
#include "../TypeDef.h"
#define MAP "map"  // class string name
#define MAP_ID GET_MODULE_ID(EUtility::eMap)  // class int id

namespace nibble {
	template <class KEYTYPE, class VALUETYPE>
	class pair {
	public:
		pair() {}
		pair(KEYTYPE key, VALUETYPE value) : first(key), second(value) {}
		inline bool operator==(const pair& rhs) { return (first == rhs.first && second == rhs.second); }
		inline bool operator!=(const pair& rhs) { return !(first == rhs.first && second == rhs.second); }

		KEYTYPE first;
		VALUETYPE second;
	};
	template <class KEYTYPE, class VALUETYPE>
	pair<KEYTYPE, VALUETYPE> make_pair(KEYTYPE keyType, VALUETYPE valueType) {
		pair<KEYTYPE, VALUETYPE> pairValue;
		pairValue.first = keyType;
		pairValue.second = valueType;
		return pairValue; 
	}

	template <class KEYTYPE, class VALUETYPE>
	class mapIterator {
	private:
		pair<KEYTYPE, VALUETYPE>* pElement;
	public:
		mapIterator(pair<KEYTYPE, VALUETYPE>* pElement) : pElement(pElement) {}

		inline mapIterator operator++() {
			mapIterator current = *this; pElement++;
			return current;
		}
		inline pair<KEYTYPE, VALUETYPE>& operator*() { return *pElement; }
		inline pair<KEYTYPE, VALUETYPE>* operator->() { return pElement; }
		inline bool operator==(const mapIterator& rhs) { return pElement == rhs.pElement; }
		inline bool operator!=(const mapIterator& rhs) { return pElement != rhs.pElement; }
	};

	template <class KEYTYPE, class VALUETYPE, int MAXLENGTH = 20>
	class map {
	private:
		int length;
		pair<KEYTYPE, VALUETYPE> elements[MAXLENGTH];
	public:
		typedef mapIterator<KEYTYPE, VALUETYPE> iterator;

		map(): length(0) {}
		virtual ~map() {}

		inline iterator begin() { return iterator(this->elements); }
		inline iterator end() { return iterator(this->elements + this->length); }

		inline int size() const { return this->length; }
		inline int max_size() const { return MAXLENGTH; }
		inline bool empty() { return (length==0)? true: false; }
		inline void clear() { this->length = 0; }
		inline iterator first() { return Iterator(this->elements); }
		inline iterator find(KEYTYPE key) {
			iterator itr = this->begin();
			for ( ; itr != end(); ++itr) {
				if (itr->first == key) {
					return itr;
				}
			}
			return itr;
		}
		inline pair<iterator, bool> insert(pair<KEYTYPE, VALUETYPE> element) {
			if (this->length == MAXLENGTH) {
//				printf("Map::insert-OverFlow\n");
				return pair<iterator, bool>(this->end(), false);
			}
			iterator itr = this->find(element.first);
			if (itr != this->end()) {
//				printf("Map::insert-Element key duplicated\n");
				return pair<iterator, bool>(itr, false);
			}
			this->elements[this->length] = element;
			this->length++;
			return pair<iterator, bool>(iterator(this->elements[this->length-1]), true);
		}

		VALUETYPE& operator[](const KEYTYPE& key)
		{
			iterator itr = this->find(key);
			if (itr != this->end()) {
				return itr->second;
			}
			itr->first = key;
			itr->second = VALUETYPE();
			this->length++;
			return itr->second;
		}
	};
};
