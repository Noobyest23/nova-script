#ifndef NOVASCRIPT_INT_VALUE_H
#define NOVASCRIPT_INT_VALUE_H
#include "Value.h"

struct NOVASCRIPT_API NovaInt : public NovaValue {
	NovaInt(int num) : novanum(num) {};
	NovaInt(std::reference_wrapper<int> cppnum) : cppnum(cppnum) {};
	int novanum = 0;
	std::reference_wrapper<int> cppnum = novanum;
	int& Num();
	const int& CNum() const;
	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) override;
	NovaValue* Assign(NovaValue* rhs) override;
};

#endif
