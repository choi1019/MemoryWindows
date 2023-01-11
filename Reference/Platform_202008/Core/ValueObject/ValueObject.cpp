#include "ValueObject.h"

unsigned ValueObject::gCounter = 0;

ValueObject::ValueObject(ClassInfo classInfo) 
	: id(gCounter++), classInfo(classInfo) 
{
}

ValueObject::~ValueObject() {
}

ClassInfo ValueObject::getClassInfo() { 
	return this->classInfo; 
}

unsigned ValueObject::getValueObjectId() { 
	return this->id; 
}


void ValueObject::read() {
}

void ValueObject::store() {
}