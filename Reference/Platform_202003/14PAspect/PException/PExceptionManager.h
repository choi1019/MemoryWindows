//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PEXCEPTIONMANAGER
#define PEXCEPTIONMANAGER "PExceptionManager"
#include "../Typedef.h"
#define PEXCEPTIONMANAGER_ID GETCLASSID(_ePExceptionManager_)

#include "../../04Aspect/Exception/ExceptionManager.h"
#include <stdio.h>

class PExceptionManager : public ExceptionManager {
private:
	FILE *pFile;
	errno_t erroNo;
public:
	PExceptionManager(): ExceptionManager(PEXCEPTIONMANAGER) {
		erroNo = fopen_s(&this->pFile, EXCEPTION_FILENAME, "w");
	}
	~PExceptionManager() {
		fclose(this->pFile);
	}

	virtual void initialize() {
	}
	virtual void finalize() {
	}

	virtual void write(Log *pLog) {
		fprintf(this->pFile, "%d, (%d, %d), %s:%d\n", 
			pLog->getType(), pLog->getTaskId(), pLog->getComponentId(), pLog->getMethodName(), pLog->getLineNo());
	}

};

#endif