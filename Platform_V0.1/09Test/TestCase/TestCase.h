#pragma once

#ifndef TESTCASE
#define TESTCASE "TESTCASE"
#include "../Typedef.h"
#define TESTCASEID GETCLASSID(_eTestCase_)

class TestCase 
{
public:
	TestCase() {}
	virtual ~TestCase() {}

	virtual void initialize() {}
	virtual void finalize() {}

	virtual void run() {}
};

#endif

