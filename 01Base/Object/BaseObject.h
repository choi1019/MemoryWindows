/////////////////////////////////////////////////////////////////
/// <summary>
/// Sungwoon Choi 2023-01-26
/// </summary>
/////////////////////////////////////////////////////////////////

#ifndef BASEOBJECT
#define BASEOBJECT

#include <01Base/typedef.h>
#define _BASEOBJECT_Id _GET_CLASS_UID(_ELayer_Base::_eBaseObject)
#define _BASEOBJECT_Name "BaseObject"

//#include "../../01Base/Aspect/Log.h"
#include <01Base/Object/RootObject.h>
#include <01Base/Memory/IMemory.h>
#include <stdio.h>

class BaseObject : public RootObject {
public:
	enum class EState {
		eCreated,
		eInitialized,
		eStarted,
		eStopped,
		eFinalized,
		eDeleted,
		eEnd
	};

private:
	// attributes
	unsigned 	m_uObjectId;
	unsigned 	m_nClassId;
	const char* m_pcClassName;
	size_t 		m_szThis;
	EState 		m_eState;

public:
	// getters and setters
	inline int GetObjectId() { return this->m_uObjectId; }
	inline int GetClassId() { return this->m_nClassId; }
	inline const char* GetClassName() { return this->m_pcClassName; }

	inline size_t GetSzThis() { return this->m_szThis; }
	inline void SetSzThis(size_t szThis) { this->m_szThis = szThis; }
	inline EState GetEState() { return this->m_eState; }
	inline void SetEState(EState eState) { this->m_eState = eState; }

public:
	// class variable
	static unsigned s_uCounter;
	static IMemory* s_pMemory;

	void* operator new (size_t szThis, const char* sMessage);
	void* operator new[] (size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete[](void* pObject);

	// dummy
	void operator delete(void* pObject, const char* sMessage);
	void operator delete[](void* pObject, const char* sMessage);

public:
	// constructors & destructors
	BaseObject(
		int nClassId = _BASEOBJECT_Id,
		const char* pcClassName = _BASEOBJECT_Name)
		: RootObject(nClassId, pcClassName)
		, m_nClassId(nClassId)
		, m_pcClassName(pcClassName)
		, m_eState(EState::eCreated)
		, m_szThis(0)
	{
	}
	virtual ~BaseObject() {
		this->m_eState = EState::eDeleted;
	}

	virtual void Initialize() {
		RootObject::Initialize();
		this->m_eState = EState::eInitialized;
	}
	virtual void Finalize() {
		RootObject::Finalize();
		this->m_eState = EState::eFinalized;
	}

	// cloneable
	virtual BaseObject* Clone() {
		return nullptr;
	}

	// serializable
	virtual char* Serialize() {
		return nullptr;
	}
	virtual void DeSerialize(char* pBuffer) {
	}
};
#endif