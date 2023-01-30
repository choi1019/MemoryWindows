#pragma once

#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject13 : public BaseObject {
private:
	Vector<int, 100>* m_pVector;  // 248 Byte

public:
	DomainObject13() : BaseObject() {
		this->m_pVector = new("DomainObject13::m_pVector") Vector<int, 100>();
	}
	virtual ~DomainObject13() {
		delete this->m_pVector;
	}

	void Run() {
		LOG_HEADER("DomainObject13::Run", m_pVector->Max_size());
		for (int i = 0; i < m_pVector->Max_size(); i++) {
			this->m_pVector->Add(i);
		}
//		for (int i = 0; i < m_pVector->Max_size(); i++) {
//			LOG((*m_pVector)[i]);
//		}

		LOG_FOOTER("DomainObject13::Run");
	}
};
