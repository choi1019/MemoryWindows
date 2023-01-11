#pragma once
#include "../TypeDef.h"
#define STRING "string"  // class string name
#define STRING_ID GET_MODULE_ID(EUtility::eString)  // class int id

#include "00Global/ValueObject/ValueObject.h"

#define MAXLENGTH_STRING 50
#define EOS '\0'

namespace nibble {
	class string : public ValueObject {
	private:
		char lCharArray[MAXLENGTH_STRING + 1];
		unsigned uLength;

		void copy(const char* rCharArray, unsigned uLength) {
			memcpy_s(this->lCharArray, uLength, rCharArray, uLength);
			this->uLength = uLength;
			this->lCharArray[this->uLength] = EOS;
		}
		void append(const char* rCharArray, unsigned uLength) {
			memcpy_s(lCharArray + this->uLength, uLength, rCharArray, uLength);
			this->uLength += uLength;
			this->lCharArray[this->uLength] = EOS;
		}
		bool equal(const char* rCharArray) const {
			for (const char* pElement = this->lCharArray; pElement < (this->lCharArray + this->uLength); pElement++) {
				if (*pElement != *rCharArray) {
					return false;
				}
				rCharArray++;
			}
			return true;
		}

	public:
		string(const char* rCharArray = "", ClassInfo classInfo = make_classInfo(STRING_ID, STRING)) : ValueObject(classInfo) {
			unsigned uLength = this->computeLength(rCharArray);
			this->copy(rCharArray, uLength);
		}
		string(const string& rString, ClassInfo classInfo = make_classInfo(STRING_ID, STRING)) : ValueObject(classInfo) {
			unsigned uLength = rString.length();
			this->copy(rString.c_str(), uLength);
		}
		~string() {}

		static unsigned computeLength(const char* charArray) {
			unsigned lengthCharArray = 0;
			for (const char* pElement = charArray; *pElement != EOS; pElement++)
			{
				if (lengthCharArray < MAXLENGTH_STRING) {
					// over flow
					break;
				}
				lengthCharArray++;
			}
			return lengthCharArray;
		}

		const unsigned length() const {
			return this->uLength;
		}
		const char* c_str() const {
			return const_cast<const char*>(this->lCharArray);
		}

		string& operator=(const char* rCharArray) {
			unsigned uLength = this->computeLength(rCharArray);
			this->copy(rCharArray, uLength);
			return *this;
		}
		string& operator=(const string& rString) {
			unsigned uLength = rString.length();
			this->copy(rString.c_str(), uLength);
			return *this;
		}
		string& append(const char* rCharArray) {
			int uLength = computeLength(rCharArray);
			this->append(rCharArray, uLength);
			return *this;
		}
		string& append(const string& rString) {
			unsigned uLength = rString.length();
			this->append(rString.c_str(), uLength);
			return *this;
		}
		char operator[](int index) const
		{
			return *(this->lCharArray + index);
		}

		char& operator[](int index)
		{
			return *(this->lCharArray + index);
		}
		bool operator==(const char* rCharArray) const {
			return this->equal(rCharArray);
		}
		bool operator==(const string& rString) const {
			return this->equal(rString.c_str());
		}
	};
};
