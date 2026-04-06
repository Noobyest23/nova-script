#ifndef NOVASCRIPT_OBJECT_H
#define NOVASCRIPT_OBJECT_H
#include "Value.h"
// this class LIES, it is 104 bytes
struct NOVASCRIPT_API NovaObject : public NovaValue {
	NovaObject() { accessables = new std::unordered_map<std::string, NovaValue*>; }

	NovaValue* Copy() override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const { return nullptr; };
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) { return nullptr; };

	void PushBack(const std::string& str, NovaValue* value);
	NovaValue* Assign(NovaValue*) override;

protected:

	void OnDestroy() override;

};

#endif
