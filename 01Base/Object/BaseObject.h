#pragma once
#include "../typedef.h"
#define _BaseObject_Id _GET_CLASS_UID(_ELayer_Base::_eBaseObject)
#define _BaseObject_Name "BaseObject"

//#include "../../01Base/Aspect/Log.h"
#include "../../01Base/Object/RootObject.h"
#include "../../01Base/Memory/IMemory.h"
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
	EState eState;

public:
	// custom memory manager
	static IMemory* s_pMemory;

	void* operator new (size_t szThis, const char* sMessage) {
		printf("\n\n@BaseObject::new %s (%zu)", sMessage, szThis);
		void* pAllocated = s_pMemory->SafeMalloc(szThis, sMessage);
		return pAllocated;
	}
	void operator delete(void* pObject) {
		printf("\n@BaseObject::delete %zu\n", (size_t)pObject);
		s_pMemory->SafeFree(pObject);
	}
	// dummy
	void operator delete(void* pObject, const char* pcName) {
		s_pMemory->SafeFree(pObject);
	}

	// constructors & destructors
	BaseObject(
		int nClassId = _BaseObject_Id,
		const char* pClassName = _BaseObject_Name)
		: RootObject(nClassId, pClassName), eState(EState::eCreated)
	{
	}
	virtual ~BaseObject() {
		this->eState = EState::eDeleted;
	}

	virtual void Initialize() {
		RootObject::Initialize();
		this->eState = EState::eInitialized;
	}
	virtual void Finalize() {
		RootObject::Finalize();
		this->eState = EState::eFinalized;
	}

	virtual EState GetEState() { return this->eState; }
	virtual void SetEState(EState eState) { this->eState = eState; }

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
