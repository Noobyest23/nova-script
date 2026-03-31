#ifndef NOVASCRIPT_STRING_VALUE_H
#define NOVASCRIPT_STRING_VALUE_H
#include "Value.h"

struct NovaInt;
struct NovaBool;

struct NOVASCRIPT_API NovaString : public NovaValue {
	NovaString(const std::string& str);
	std::string str;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;
	
	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) override;
	NovaValue* Assign(NovaValue* rhs) override;
};


#endif