#ifndef NOVASCRIPT_VALUE_BOOL_H
#define NOVASCRIPT_VALUE_BOOL_H

#include "Value.h"

struct NOVASCRIPT_API NovaBool : public NovaValue {
	NovaBool(bool b) : novab(b) {};
	NovaBool(bool* cppb) : cppb(cppb) {};

	bool novab = false;
	bool* cppb = nullptr;

	bool* B();
	const bool& CB() const;

	NovaValue* Copy() override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) { return nullptr; };
	NovaValue* Assign(NovaValue* rhs) override;

protected:

	void OnDestroy() override;

};

#endif