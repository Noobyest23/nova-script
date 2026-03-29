#ifndef NOVASCRIPT_STRING_VALUE_H
#define NOVASCRIPT_STRING_VALUE_H
#include "Value.h"

struct NovaString : public NovaValue {
	NovaString(const std::string& str) : str(str) {};
	std::string str;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
};


#endif