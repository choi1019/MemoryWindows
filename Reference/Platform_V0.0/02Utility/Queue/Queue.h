//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef QUEUE
#define QUEUE "_Queue"
#include "../TypeDef.h"
#define QUEUE_ID GETCLASSID(_eQueue_)

template <class TYPE, int MAXLENGTH = 20>
class _Queue {
protected:
	TYPE elements[MAXLENGTH];
	int first, last, length;
public:
	_Queue(): first(0), last(0), length(0) {}
	~_Queue() {}

	inline int getLength() { return this->length; }
	inline int getMaxLength() { return MAXLENGTH; }
	inline bool empty() { return (length == 0);	}
	inline bool full() { return (!(length < MAXLENGTH)); }

	inline void clear() {
		this->first = 0;
		this->last = 0;
		this->length = 0;
	}
	inline void enQueue(TYPE element) {
		this->elements[this->last] = element;
		this->last++;
		this->last %= MAXLENGTH;
		this->length++;
	}
	inline TYPE deQueue() {
		TYPE element;
		element = this->elements[this->first];
		this->first++;
		this->first %= MAXLENGTH;
		this->length--;
		return element;
	}
};

#endif /* QUEUE */
