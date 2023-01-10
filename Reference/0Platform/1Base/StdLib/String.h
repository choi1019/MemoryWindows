#pragma once

#include "../TypeDef.h"
#define _String_Id _GET_CLASS_UID(_ELayer_Base::_eString)
#define _String_Name "string"

#include "../../1Base/Object/BaseObject.h"

#include <time.h>
#include <string.h>

#define EOS '\0'
#define MAXLENGTH_STRING 200

//template<int MAXLENGTH_STRING = 200>
class String : public BaseObject {
private:
	char m_lCharArray[MAXLENGTH_STRING + 1];
	unsigned m_uLength;

	inline void copy(const char* rCharArray, unsigned uLength) {
		memcpy_s(this->m_lCharArray, uLength, rCharArray, uLength);
		this->m_uLength = uLength;
		this->m_lCharArray[this->m_uLength] = EOS;
	}
	inline void append(const char* rCharArray, unsigned uLength) {
		memcpy_s(m_lCharArray + this->m_uLength, uLength, rCharArray, uLength);
		this->m_uLength += uLength;
		this->m_lCharArray[this->m_uLength] = EOS;
	}
	inline bool equal(const char* rCharArray) const {
		unsigned uLength = this->computeLength(rCharArray);
		unsigned uCnt = 0;
		for (const char* pElement = this->m_lCharArray; pElement < (this->m_lCharArray + this->m_uLength); pElement++) {
			if (*pElement != *rCharArray) {
				return false;
			}
			rCharArray++;
			uCnt++;
		}
		if (uCnt != uLength) return false;
		return true;
	}
	inline void clearData() {
		this->m_uLength = 0;
		this->m_lCharArray[this->m_uLength] = EOS;
	}

	inline void Reverse(const char* rCharArray, size_t szLength) {
		for (size_t i = 0; i < szLength; i++) {
			this->m_lCharArray[szLength - 1 - i] = rCharArray[i];
		}
		this->m_uLength = static_cast<unsigned>(szLength);
		this->m_lCharArray[this->m_uLength] = EOS;
	}
	inline void IToA(int nValue) {
		char cArray[MAXLENGTH_STRING + 1];
		size_t szLength = 0;
		for (int quotient = nValue; quotient > 0 && szLength < MAXLENGTH_STRING; quotient = quotient / 10) {
			cArray[szLength++] = quotient % 10 + '0';
		}
		this->Reverse(cArray, szLength);
	}

	inline void IToA(size_t nValue) {
		char cArray[MAXLENGTH_STRING + 1];
		size_t szLength = 0;
		for (size_t quotient = nValue; quotient > 0 && szLength < MAXLENGTH_STRING; quotient = quotient / 10) {
			cArray[szLength++] = quotient % 10 + '0';
		}
		this->Reverse(cArray, szLength);
	}

public:
	String() : BaseObject(_String_Id, _String_Name)
	{
		this->copy("", 0);
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
	String(const int nValue) : BaseObject(_String_Id, _String_Name)
	{
		IToA(nValue);
	}

	String(const unsigned int uValue) : BaseObject(_String_Id, _String_Name)
	{
		IToA(static_cast<size_t>(uValue));
	}

	String(const size_t nValue) : BaseObject(_String_Id, _String_Name)
	{
		IToA(nValue);
	}
	/*
	String(const float fValue) : Collection(_String_Id, _String_Name)
	{
		char rCharArray[MAXLENGTH_STRING + 1];
		_gcvt_s(rCharArray, MAXLENGTH_STRING, fValue, 5);
		unsigned uLength = computeLength(rCharArray);
		this->copy(rCharArray, uLength);
	}
	*/
	String(time_t tValue) : BaseObject(_String_Id, _String_Name)
	{
		char rCharArray[MAXLENGTH_STRING + 1];
		ctime_s(rCharArray, MAXLENGTH_STRING, &tValue);
		unsigned uLength = computeLength(rCharArray);
		this->copy(rCharArray, uLength);
	}
	~String() {}
	virtual void Initialize() { BaseObject::Initialize(); }
	virtual void Finalize() { BaseObject::Finalize(); }

	static unsigned computeLength(const char* charArray) {
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

	inline const unsigned length() const {
		return this->m_uLength;
	}
	inline const char* c_str() const {
		return const_cast<const char*>(this->m_lCharArray);
	}
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
	inline bool operator==(const char* rCharArray) const {
		return this->equal(rCharArray);
	}
	inline bool operator==(const String& rString) const {
		return this->equal(rString.c_str());
	}
	inline void clear()
	{
		this->clearData();
	}

	/*
	
	inline String& assign(const String& rString)
	{
		*this = rString;
		return *this;
	}
	inline String& assign(const char* rCharArray) {
		*this = rCharArray;
		return *this;
	}
	inline String& assign(const char* rCharArray, unsigned uLength) {
		unsigned uLengthTemp = uLength > MAXLENGTH_STRING ? MAXLENGTH_STRING : uLength;
		this->copy(rCharArray, uLengthTemp);
		return *this;
	}

	String& append(const char* rCharArray) {
		this->append(rCharArray, computeLength(rCharArray));
		return *this;
	}
	String& append(const String& rString) {
		this->append(rString.c_str(), rString.length());
		return *this;
	}
	char operator[](int index) const
	{
		return *(this->m_lCharArray + index);
	}

	char& operator[](int index)
	{
		return *(this->m_lCharArray + index);
	}


	const char& at(unsigned uPos)
	{
		return *(this->m_lCharArray + uPos);
	}

	String& insert(unsigned nPos, const String& rString)
	{
		size_t size = (size_t)this->m_uLength + (size_t)rString.length();
		if (size > MAXLENGTH_STRING) return *this;

		String strTemp;
		strTemp.assign(this->c_str(), nPos);

		String strTemp1(rString);
		strTemp1.append(this->m_lCharArray + nPos, this->m_uLength - nPos);

		*this = strTemp;
		this->append(strTemp1);

		return *this;
	}
	String& insert(unsigned uPos, const char* rCharArray)
	{
		size_t size = (size_t)this->m_uLength + (size_t)this->computeLength(rCharArray);
		if (size > MAXLENGTH_STRING) return *this;

		String strTemp;
		strTemp.assign(this->c_str(), uPos);

		String strTemp1(rCharArray);
		strTemp1.append(this->m_lCharArray + uPos, this->m_uLength - uPos);

		*this = strTemp;
		this->append(strTemp1);

		return *this;
	}

	String substr(unsigned uPos, unsigned uLength) const
	{
		String strTemp;
		strTemp.assign(this->c_str() + uPos, uLength);

		return strTemp;
	}

	unsigned find(const String& rString) const {
		unsigned nPos = 0;
		for (const char* pElement = this->m_lCharArray; pElement < (this->m_lCharArray + this->m_uLength); pElement++) {
			if (*pElement == *rString.c_str()) {
				String strTemp;
				strTemp.assign(pElement, rString.length());
				if (strTemp == rString) {
					return nPos;
				}
			}
			nPos++;
		}

		return -1;
	}

	unsigned find(const char* rCharArray) const
	{
		unsigned uLength = this->computeLength(rCharArray);
		unsigned nPos = 0;
		for (const char* pElement = this->m_lCharArray; pElement < (this->m_lCharArray + this->m_uLength); pElement++) {
			if (*pElement == *rCharArray) {
				String strTemp;
				strTemp.assign(pElement, uLength);
				if (strTemp == rCharArray) {
					return nPos;
				}
			}
			nPos++;
		}

		return -1;
	}

	String& replace(unsigned uPos, unsigned uLength, const String& rString)
	{
		String strTemp;
		strTemp.assign(this->c_str(), uPos);
		strTemp.append(rString);

		strTemp.append(this->c_str() + uPos + uLength);

		*this = strTemp;

		return *this;
	}

	String& replace(unsigned uPos, unsigned uLength, const char* rCharArray)
	{
		String strTemp;
		strTemp.assign(this->c_str(), uPos);
		strTemp.append(rCharArray);

		strTemp.append(this->c_str() + uPos + uLength);

		*this = strTemp;

		return *this;
	}

	String& erase(unsigned nPos, unsigned uLength)
	{
		String strTemp;
		strTemp.assign(this->c_str(), nPos);

		strTemp.append(this->m_lCharArray + nPos + uLength);

		*this = strTemp;

		return *this;
	}
	*/
};
