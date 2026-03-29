#ifndef NOVASCRIPT_NUM_VALUE_H
#define NOVASCRIPT_NUM_VALUE_H
#include "Value.h"

struct NovaFloat : public NovaValue {
	NovaFloat(float num) : num(num) {};
	float num;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
};


#endif
