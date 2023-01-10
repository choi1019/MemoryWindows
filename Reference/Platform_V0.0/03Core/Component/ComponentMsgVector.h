//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef COMPONENTMSGVECTOR
#define COMPONENTMSGVECTOR "ComponentMsgVector"
#include "../typedef.h"
#define COMPONENTMSGVECTOR_ID GETCLASSID(_eComponentMsgVector_)

#include "../../01Global/Object/Object.h"
#include "../../02Utility/Vector/Vector.h"
#include "../../03Core/component/Message.h"

class ComponentMsgVector : public Object {
public:
	ComponentMsgVector() : Object(Make_ClassInfo(COMPONENTMSGVECTOR_ID, COMPONENTMSGVECTOR)) {}
	virtual ~ComponentMsgVector() {}
	virtual inline void initialize() { Object::initialize(); }
	virtual inline void finalize() { Object::finalize(); }

	virtual inline _VectorIterator<Message *> begin() = 0;
	virtual inline _VectorIterator<Message *> end() = 0;
	virtual inline void add(Message *pMsg) = 0;
	virtual inline void clear() = 0;
	virtual inline unsigned getLength() = 0;
};

#endif
