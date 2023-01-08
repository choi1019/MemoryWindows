//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef DOMAIN_TYPEDEF
#define DOMAIN_TYPEDEF  "DOMAIN_TYPEDEF"
#include "../../01Global/TypeDef.h"

enum EDomain {
	_eDomainStart_ = GETLAYERID(__eDomain__),
	_eSender_,
	_eReceiver_,
	_eDomainLayerEnd_
};

#endif