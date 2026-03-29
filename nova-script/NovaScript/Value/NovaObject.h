#ifndef NOVASCRIPT_OBJECT_H
#define NOVASCRIPT_OBJECT_H
#include "Value.h"
// this class LIES, it is 104 bytes
struct NovaObject : public NovaValue {
	NovaObject() { accessables = new std::unordered_map<std::string, NovaValue*>; }

	NovaValue* Copy() const;
	std::string ToString() const;
	std::string Type() const;

	void PushBack(const std::string& str, NovaValue* value);

};

#endif
