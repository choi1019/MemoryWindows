//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PTESTSUIT
#define PTESTSUIT "PTESTSUIT"
#include "../Typedef.h"
#define PTESTSUIT_ID GETCLASSID(_ePTestuit_)

#include "../../09Test/TestSuit/TestSuit.h"
#include "../../19PTest/PTestCase/PTestCase.h"

class PTestSuit : public TestSuit {
private:
	PTestCase* pTestCase;

public:
	PTestSuit() {
		this->pTestCase = new PTestCase();
	}
	virtual ~PTestSuit() {
		delete this->pTestCase;
	}

	virtual void initialize() {
		this->pTestCase->initialize();
	}

	virtual void finalize() {
		this->pTestCase->finalize();
	}

	virtual void run() {
		try {
			this->pTestCase->run();
		}
		catch (exception &e) {
			printf("%s\n", e.what());
		}
	}
};

#endif /* PTESTSUIT */