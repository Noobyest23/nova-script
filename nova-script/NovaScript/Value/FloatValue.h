#ifndef NOVASCRIPT_NUM_VALUE_H
#define NOVASCRIPT_NUM_VALUE_H
#include "Value.h"

struct NOVASCRIPT_API NovaFloat : public NovaValue {
	NovaFloat(float num) : novanum(num) {};
	NovaFloat(std::reference_wrapper<float> cppnum) : cppnum(cppnum) {};
	float novanum = 0;
	std::reference_wrapper<float> cppnum = novanum;
	float& Num();
	const float& CNum() const;
	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) override;
	NovaValue* Assign(NovaValue* rhs) override;
};


#endif
