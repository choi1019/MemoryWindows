//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef TESTSUIT
#define TESTSUIT "TESTSUIT"
#include "../Typedef.h"
#define TESTSUIT_ID GETCLASSID(_eTestuit_)

class TestSuit {
private:
;

public:
	TestSuit() {}
	virtual ~TestSuit() {}

	virtual void initialize() {}
	virtual void finalize() {}

	virtual void run() {}
};

#endif /* TESTSUIT */