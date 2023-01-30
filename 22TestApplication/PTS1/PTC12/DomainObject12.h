#pragma once

#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject12 : public BaseObject {
private:
	Vector<int>* m_pVector;  // 248 Byte

public:
	DomainObject12() : BaseObject() {
		this->m_pVector = new("DomainObject12::m_pVector") Vector<int>();
	}
	virtual ~DomainObject12() {
		delete this->m_pVector;
	}

	void Run() {
		LOG_HEADER("DomainObject12::Run", m_pVector->Max_size());
		for (int i = 0; i < m_pVector->Max_size(); i++) {
			m_pVector->Add(i);
		}
		LOG_FOOTER("DomainObject12::Run");
	}
};
