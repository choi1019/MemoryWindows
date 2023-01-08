//////////////////////////////////////////////////////////////
// Created 2017-01-21 by Sungwoon Choi
// All Rights Reserved
//////////////////////////////////////////////////////////////

#ifndef STRING
#define STRING "String"
#include "../TypeDef.h"
#define STRING_ID GETCLASSID(_eString_)

#define MAXSTRINGLENGTH 20
#define EOS '\0'

class String {
private:
	char charArray[MAXSTRINGLENGTH+1];
	int length;

	inline void copyArray(int position, const char* rCharArray) {
		this->length = position;
		for (const char *pElement = rCharArray; this->length < MAXSTRINGLENGTH && *pElement != EOS; pElement++) {
			this->charArray[this->length++] = *pElement;
		}
		this->charArray[this->length] = EOS;
	}
public:
	String(const char* rCharArray = "\0") {
		this->length = 0;
		this->copyArray(0, rCharArray);
	}
	~String() {}

	inline const int getLength() const {
		return this->length;
	}
	inline const char* c_str() const {
		return const_cast<const char*>(this->charArray);
	}

	inline String& operator=(const String& rString) {
		this->copyArray(0, rString.c_str());
		return *this;
	}	
	inline String& operator=(const char* rCharArray) {
		this->copyArray(0, rCharArray);
		return *this;
	}

	inline String& append(const String& rString) {
		// overflow check is omitted
		this->copyArray(this->length, rString.c_str());
		return *this;
	}
	inline String& append(const char* rCharArray) {
		// Eliminate EOS
		this->copyArray(this->length, rCharArray);
		return *this;
	}

	inline char operator[](int index)const
	{
		return *(this->charArray + index);
	}
	inline char& operator[](int index)
	{
		return *(this->charArray + index);
	}

	bool operator==(const char* rCharArray) const {
		for (const char* pElement = this->charArray;
			pElement < this->charArray + this->length && rCharArray != (const char*)EOS;
			pElement++
			) {
			if (*pElement != *rCharArray) {
				return false;
			}
			rCharArray++;
		}
		return true;
	}

	bool operator==(const String& rString) const {
		if (this->length != rString.length) 
			return false;

		const char *rCharArray = rString.charArray;
		for (const char* pElement = this->charArray;
			pElement < this->charArray + this->length;
			pElement++
			) {
			if (*pElement != *rCharArray) {
				return false;
			}
			rCharArray++;
		}
		return true;
	}
};

/*
static void printError(const char *msg, int id) {
	printf("****Error=%s(%d)\n", msg, id);
}
static void printError(const char *msg, const char *info) {
	printf("****Error=%s(%s)\n", msg, info);
}
static void printError(const char *msg, String &info) {
	printf("****Error=%s(%s)\n", msg, info.c_str());
}

static void printString(String& name) {
	printf("%s", name.c_str());
}
static void printString(const char* name) {
	printf("%s", name);
}
static void printMethod(const char *name) {
	printf("::%s", name);
}
static void printId(int id) {
	printf("(%d)", id);
}
static void printId(const char *name, int id) {
	printf("(%s=%d)", name, id);
}
static void println() {
	printf("\n");
}
*/

#endif
