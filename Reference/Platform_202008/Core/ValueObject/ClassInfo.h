#pragma once

class ClassInfo {
private:
	unsigned id;
	const char* name;
public:
	ClassInfo(const unsigned id, const char* name) : id(id), name(name) {}
	~ClassInfo() {}
	unsigned getId() { return this->id; }
	const char* getName() { return this->name; }
};

/*
static ClassInfo make_classInfo(const unsigned id, const char* pName) {
	ClassInfo classInfo(id, pName);
	return classInfo;
}
*/