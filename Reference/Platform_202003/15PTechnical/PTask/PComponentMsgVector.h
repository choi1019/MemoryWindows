//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PCOMPONENTMSGVECTOR
#define PCOMPONENTMSGVECTOR "PComponentMsgVector"
#include "../typedef.h"
#define PCOMPONENTMSGVECTOR_ID GETCLASSID(_ePComponentMsgVector_)

#include "../typedef.h"
#include "../../03Core/Component/ComponentMsgVector.h"

class PComponentMsgVector : public ComponentMsgVector {
private:
	_Vector<Message *, MAXCOMPONENTMSGVECTORLENGTH> vector;
	HANDLE *pEventHandle;

public:
	PComponentMsgVector(HANDLE *pHandle);
	~PComponentMsgVector();

	void initialize();
	void finalize();

	inline _VectorIterator<Message *> begin();
	inline _VectorIterator<Message *> end();

	inline void add(Message *pMsg);
	inline void clear();
	inline unsigned getLength();
};

#endif
