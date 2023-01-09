#pragma once

#define UNDEFINED -1

#define _GET_LAYER_UID(ID)		(int)ID*1000
#define _GET_CLASS_UID(ID)		(int)ID*1000

enum class _ELayer {
	_eBegin = 0,

	_eBase = 1,
	_ePlatform = 2,
	_eTechnical = 3,
	_eAspect = 4,

	_ePPlatform = 5,
	_ePTechnical = 6,
	_ePAspect = 7,

	_eTestPlatform = 12,
	_eTestApplication = 19,

	_eDomain = 21,
	_ePDomain = 25,
	_ePApplication = 29,

	_eEnd,
};

enum class _ELayer_Base {
	_eBegin = _GET_LAYER_UID(_ELayer::_eBase),

	// object
	_eRootObject,
	_eBaseObject,

	// memory
	_eMemory,

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

	_eEnd,
};

#undef GetClassName





