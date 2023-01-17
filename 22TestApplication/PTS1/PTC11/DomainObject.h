#pragma once

#include "../../../01Base/Object/BaseObject.h"
#include "../../../01Base/Aspect/Log.h"
#include "../../../01Base/StdLib/Vector.h"

#define SIZE_ARRARY 10
class DomainObject : public BaseObject {
private:
	int m_pArray[SIZE_ARRARY];
	Vector<int>* m_pVector;

public:
	DomainObject() : BaseObject() {
		this->m_pVector = new("") Vector<int>();

		for (int i = 0; i < SIZE_ARRARY; i++) {
			this->m_pArray[i] = i;
		}
	}
	virtual ~DomainObject() {
		delete this->m_pVector;
	}

	void Show(const char* pcTitle) {
		LOG_HEADER("DomainObject::Show", String(sizeof(*this)));
		for (int i = 0; i < SIZE_ARRARY; i++) {
			LOG(this->m_pArray[i]);
		}
		LOG_FOOTER("DomainObject");
	}

};
