//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PAIR
#define PAIR "_Pair"
#include "../TypeDef.h"
#define PAIR_ID GETCLASSID(_ePair_)

template <class KEYTYPE, class VALUETYPE>
struct _Pair {
	KEYTYPE first;
	VALUETYPE second;
};

template <class KEYTYPE, class VALUETYPE>
_Pair<KEYTYPE, VALUETYPE> Make_Pair(KEYTYPE keyType, VALUETYPE valueType) {
	_Pair<KEYTYPE, VALUETYPE> pair;
	pair.first = keyType;
	pair.second = valueType;
	return pair;
}
#endif /* PAIR */
