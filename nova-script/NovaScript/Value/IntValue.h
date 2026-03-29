#ifndef NOVASCRIPT_INT_VALUE_H
#define NOVASCRIPT_INT_VALUE_H
#include "Value.h"

struct NovaInt : public NovaValue {
	NovaInt(int num) : num(num) {};
	int num;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
};

#endif
