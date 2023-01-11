//////////////////////////////////////////////////////////////
// Created 2019-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef PMAINTASK
#define PMAINTASK  "PMainTask"
#include "../typedef.h"
#define PMAINTASK_ID GETCLASSID(_ePMainTask_)


class PMainTask {
private:

public:
	PMainTask();
	~PMainTask();
	void initialize();
	void finalize();

	// as a Main
	void startCore();
	void stopCore();

};

#endif /* TASKTHREADOS_ */
