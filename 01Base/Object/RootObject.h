/////////////////////////////////////////////////////////////////
/// <summary>
/// 모든 클래스들은 RootObject 클래스로 부터 상속 되어야 한다.
/// </summary>
/////////////////////////////////////////////////////////////////

#pragma once
#include "../typedef.h"
#define _RootObject_Id _GET_CLASS_UID(_ELayer_Base::_eRootObject)
#define _RootObject_Name "RootObject"
#undef GetClassName

class RootObject {
private:
	// class variable
	static unsigned s_uCounter;

	// attributes
	int m_uObjectId;
	int m_nClassId;
	const char* m_pcClassName;
	size_t m_szThis;

public:
	// constructors and destructors
	RootObject(
		int nClassId = _RootObject_Id,
		const char* pcClassName = _RootObject_Name)
		: m_uObjectId(s_uCounter++)
		, m_nClassId(nClassId)
		, m_pcClassName(pcClassName)
		, m_szThis(0)
	{
	}
	virtual ~RootObject() 
	{
	}
	virtual void Initialize()
	{
	}
	virtual void Finalize()
	{
	}

	// getters and setters
	inline int GetObjectId() { return this->m_uObjectId; }
	inline int GetClassId() { return this->m_nClassId; }
	inline const char* GetClassName() { return this->m_pcClassName; }

	inline size_t GetSzThis() { return this->m_szThis; }
	inline void SetSzThis(size_t szThis) { this->m_szThis = szThis; }
};


