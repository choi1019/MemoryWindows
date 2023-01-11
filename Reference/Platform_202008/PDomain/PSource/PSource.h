#pragma once
#include "../TypeDef.h"
#define PSOURCE "PSource"  // class string name
#define PSOURCE_ID GET_MODULE_ID(EPDomain::ePSource)  // class int id

#include "../../../../Domain_Lib/Source/07Domain/Source/Source.h"

class PSource : public Source {

protected:
	virtual void sense() {
//		cout << _methodName() << _endl;
	}

public:
	PSource(ClassInfo classInfo = ClassInfo(PSOURCE_ID, PSOURCE))
		:Source(classInfo)
	{}
	virtual ~PSource() {}
};
