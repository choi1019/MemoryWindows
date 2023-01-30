#pragma once

#include <01Base/typedef.h>
#define _String_Id _GET_CLASS_UID(_ELayer_Base::_eString)
#define _String_Name "String"

#include <01Base/Object/BaseObject.h>
#include <string.h>

#define EOS '\0'
#define MAXLENGTH_STRING 200

//template<int MAXLENGTH_STRING = 200>
class String : public BaseObject {
private:
	const char hdigit[16] = { '0', '1', '2', '3', '4', '5' , '6', '7', '8' , '9', 'a', 'b' , 'c', 'd', 'e' };
	unsigned m_uLength;
	char m_lCharArray[MAXLENGTH_STRING + 1];

public:
	// setters and getters
	inline const unsigned length() const {
		return this->m_uLength;
	}
	inline const char* c_str() const {
		return const_cast<const char*>(this->m_lCharArray);
	}

private:

	inline unsigned computeLength(const char* charArray) {
		if (charArray == nullptr) return 0;

		unsigned lengthCharArray = 0;
		for (const char* pElement = charArray; *pElement != EOS; pElement++)
		{
			if (lengthCharArray >= MAXLENGTH_STRING) {
				// over flow
				break;
			}
			lengthCharArray++;
		}
		return lengthCharArray;
	}

	inline bool equal(const char* rCharArray) {
		unsigned uLength = this->computeLength(rCharArray);
		if (uLength != m_uLength) {
			return false;
		} else {
			unsigned uCnt = 0;
			for (const char* pElement = this->m_lCharArray; pElement < (this->m_lCharArray + this->m_uLength); pElement++) {
				if (*pElement != *rCharArray) {
					return false;
				}
				rCharArray++;
				uCnt++;
			}
			return true;
		}
	}
	inline void copy(const char* rCharArray, unsigned uLength) {
		if (uLength > (MAXLENGTH_STRING - 1)) {
			// overflow
		} else {
			memcpy(this->m_lCharArray, rCharArray, uLength);
			this->m_uLength = uLength;
			this->m_lCharArray[this->m_uLength] = EOS;
		}
	}
	inline void append(const char* rCharArray, unsigned uLength) {
		if ((m_uLength + uLength) > (MAXLENGTH_STRING - 1)) {
			// overflow
		} else {
			memcpy(m_lCharArray + this->m_uLength, rCharArray, uLength);
			this->m_uLength += uLength;
			this->m_lCharArray[this->m_uLength] = EOS;
		}
	}
	inline void reverse(const char* rCharArray, size_t szLength) {
		for (size_t i = 0; i < szLength; i++) {
			this->m_lCharArray[szLength - 1 - i] = rCharArray[i];
		}
		this->m_uLength = static_cast<unsigned>(szLength);
		this->m_lCharArray[this->m_uLength] = EOS;
	}
	inline void clearArray() {
		this->m_uLength = 0;
		this->m_lCharArray[this->m_uLength] = EOS;
	}

	inline void iToa(int nValue) {
		if (nValue == 0) {
			m_uLength = 1;
			m_lCharArray[0] = '0';
			m_lCharArray[1] = EOS;
		}
		else {
			char cArray[MAXLENGTH_STRING + 1];
			size_t szLength = 0;
			for (int quotient = nValue; quotient > 0 && szLength < MAXLENGTH_STRING; quotient = quotient / 10) {
				cArray[szLength++] = hdigit[quotient % 10];
			}
			this->reverse(cArray, szLength);
		}
	}

	inline void sToa(size_t nValue) {
		if (nValue == 0) {
			m_uLength = 1;
			m_lCharArray[0] = '0';
			m_lCharArray[1] = EOS;
		}
		else {
			char cArray[MAXLENGTH_STRING + 1];
			size_t szLength = 0;
			for (size_t quotient = nValue; quotient > 0 && szLength < MAXLENGTH_STRING; quotient = quotient / 10) {
				cArray[szLength++] = hdigit[quotient % 10];
			}
			this->reverse(cArray, szLength);
		}
	}
	inline void bToa(bool bValue) {
		if (bValue) {
			m_uLength = 1;
			m_lCharArray[0] = 't';
			m_lCharArray[1] = EOS;
		}
		else {
			m_uLength = 1;
			m_lCharArray[0] = 'f';
			m_lCharArray[1] = EOS;
		}
	}

public:
	String() : BaseObject(_String_Id, _String_Name)
	{
		this->m_uLength = 0;
	}
	String(const String& rString) : BaseObject(_String_Id, _String_Name)
	{
		unsigned uLength = rString.length();
		this->copy(rString.c_str(), uLength);
	}
	String(const char* rCharArray) : BaseObject(_String_Id, _String_Name)
	{
		unsigned uLength = this->computeLength(rCharArray);
		this->copy(rCharArray, uLength);
	}
	String(const size_t nValue) : BaseObject(_String_Id, _String_Name)
	{
		sToa(nValue);
	}
	String(const int nValue) : BaseObject(_String_Id, _String_Name)
	{
		iToa(nValue);
	}
	String(const unsigned int uValue) : BaseObject(_String_Id, _String_Name)
	{
		sToa(static_cast<size_t>(uValue));
	}
	String(const bool bValue) : BaseObject(_String_Id, _String_Name)
	{
		bToa(bValue);
	}
	String(const void* pValue) : BaseObject(_String_Id, _String_Name)
	{
		sToa((size_t)pValue);
	}
	/*
	String(const float fValue) : Collection(_String_Id, _String_Name)
	{
		char rCharArray[MAXLENGTH_STRING + 1];
		_gcvt_s(rCharArray, MAXLENGTH_STRING, fValue, 5);
		unsigned uLength = computeLength(rCharArray);
		this->copy(rCharArray, uLength);
	}
	String(time_t tValue) : BaseObject(_String_Id, _String_Name)
	{
		char rCharArray[MAXLENGTH_STRING + 1];
		ctime_s(rCharArray, MAXLENGTH_STRING, &tValue);
		unsigned uLength = computeLength(rCharArray);
		this->copy(rCharArray, uLength);
	}
	*/
	~String() {}
	virtual void Initialize() { BaseObject::Initialize(); }
	virtual void Finalize() { BaseObject::Finalize(); }

	inline String& operator=(const char* rCharArray) {
		unsigned uLength = this->computeLength(rCharArray);
		this->copy(rCharArray, uLength);
		return *this;
	}
	inline String& operator=(const String& rString) {
		unsigned uLength = rString.length();
		this->copy(rString.c_str(), uLength);
		return *this;
	}
	inline String& operator+ (const char* rCharArray) {
		this->append(rCharArray, computeLength(rCharArray));
		return *this;
	}
	inline String& operator+ (const String& rString) {
		this->append(rString.c_str(), rString.length());
		return *this;
	}
	inline String& operator<< (const char* rCharArray) {
		this->append(rCharArray, computeLength(rCharArray));
		return *this;
	}
	inline String& operator<< (const String& rString) {
		this->append(rString.c_str(), rString.length());
		return *this;
	}
	inline bool operator==(const char* rCharArray) {
		return this->equal(rCharArray);
	}
	inline bool operator==(const String& rString) {
		return this->equal(rString.c_str());
	}
	inline void clear()
	{
		this->clearArray();
	}
};