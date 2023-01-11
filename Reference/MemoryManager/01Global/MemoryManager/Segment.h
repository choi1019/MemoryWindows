//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef SEGMENT
#define SEGMENT "Segment"
#include "../TypeDef.h"
#define SEGMENT_ID GETCLASSID(_eSegment_)

class Segment {
public:
	enum EErrorCode {
		eErrorCodeStart = SEGMENT_ID,
		eNotInitialized,
		eOverflow,
		eErrorCodeENd
	};

private:
	// global counter
	static unsigned sSegmentId;

	unsigned id;
	const char *name;
	char *pStartAddress;
	unsigned maxSize;
	char *pCurrentAddress;

public:
	Segment(const char *name, char *pStartAddress, unsigned maxSize);
	virtual ~Segment();

	void initialize();
	void finalize();

	inline long long getCurrentSize();
	inline long getMaxSize();

	void* allocate(size_t size);
};

#endif