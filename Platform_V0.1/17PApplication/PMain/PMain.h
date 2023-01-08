#pragma once

#include "../../07Application/Main/Main.h"
#include "../../19PTest/PTestSuit/PTestSuit.h"

class PMain : public Main
{
private:
	PTestSuit *pTestSuit;
public:
	PMain() {
		Memory::initialize();
		this->pTestSuit = new PTestSuit();
	}
	virtual ~PMain() {
		delete this->pTestSuit;
	}

	virtual void initialize() {
		this->pTestSuit->initialize();
	}

	virtual void finalize() {
		this->pTestSuit->finalize();
	}

	virtual void run() {
		this->pTestSuit->run();
	}
};
