#pragma once

#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject14 : public BaseObject {
private:
	Vector<int, 200>* m_pVector;  // 248 Byte

public:
	DomainObject14() : BaseObject() {
		this->m_pVector = new("DomainObject14::m_pVector") Vector<int, 200>();
	}
	virtual ~DomainObject14() {
		delete this->m_pVector;
	}

	void Run() {
		LOG_HEADER("DomainObject14::Run", m_pVector->Max_size());
		for (int i = 0; i < m_pVector->Max_size(); i++) {
			this->m_pVector->Add(i);
		}
//		for (int i = 0; i < m_pVector->Max_size(); i++) {
//			LOG((*m_pVector)[i]);
//		}

		LOG_FOOTER("DomainObject14::Run");
	}
};
