#ifndef NOVASCRIPT_VALUE_BOOL_H
#define NOVASCRIPT_VALUE_BOOL_H

#include "Value.h"

struct NOVASCRIPT_API NovaBool : public NovaValue {
	NovaBool(bool b) : b(b) {};
	bool b;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
	NovaValue* Assign(NovaValue* rhs) override;
};

#endif