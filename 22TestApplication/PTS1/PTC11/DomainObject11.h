#pragma once

#include <01Base/Object/BaseObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

class DomainObject11 : public BaseObject {
private:
	Vector<int>* m_pVector;  // 248 Byte

public:
	DomainObject11() 
		: BaseObject()
		, m_pVector(0)
	{
		this->m_pVector = new("DomainObject11::m_pVector") Vector<int>();
	}
	virtual ~DomainObject11() {
		delete this->m_pVector;
	}

	void Run() {
		LOG_HEADER("DomainObject11::Run", this->m_pVector->Max_size());
		for (int i = 0; i < this->m_pVector->Max_size(); i++) {
			this->m_pVector->Add(i);
		}

		for (int i = 0; i < m_pVector->Max_size(); i++) {
			LOG_NEWLINE((*m_pVector)[i]);
		}
		LOG_FOOTER("DomainObject11::Run");
	}
};
