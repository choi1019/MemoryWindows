#pragma once

#include <typedef.h>

enum class _ELayer_Base {
	_eBegin = _GET_LAYER_UID(_ELayer::_eBase),

	// object
	_eRootObject,
	_eBaseObject,

	// memory
	_eIMemory,

	// stdlib
	_eCollection,
	_eVector,
	_eMap,
	_eQueue,
	_eString,

	// aspect
	_eAspect,
	_eException,
	_eLog,
	_eDirectory,

	_eEnd
};



