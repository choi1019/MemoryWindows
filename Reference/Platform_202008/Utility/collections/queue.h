#pragma once
#include "../TypeDef.h"
#define QUEUE "queue"  // class string name
#define QUEUE_ID GET_MODULE_ID(EUtility::eQueue)  // class int id

namespace nibble {
	template <class ELEMENTTYPE, int MAXLENGTH = 20>
	class queue {
	protected:
		ELEMENTTYPE elements[MAXLENGTH];
		int first, last, length;
	public:
		queue(): first(0), last(0), length(0) {}
		virtual ~queue() {}

		inline int size() { return this->length; }
		inline bool empty() { return (length==0)? true: false; }
		inline bool full() { return (length < MAXLENGTH) ? false : true; }
		inline void clear() {
			this->first = 0; 
			this->last = 0;
			this->length = 0;
		}
		inline void push(ELEMENTTYPE element) {
			this->elements[this->last] = element;
			this->last++;
			this->last %= MAXLENGTH;
			this->length++;
		}
		inline ELEMENTTYPE front() {
			ELEMENTTYPE element;
			element = this->elements[this->first];
			return element;
		}
		inline void pop() {
			this->first++;
			this->first %= MAXLENGTH;
			this->length--;
		}
	};
}

