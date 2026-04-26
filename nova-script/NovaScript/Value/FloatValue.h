#ifndef NOVASCRIPT_NUM_VALUE_H
#define NOVASCRIPT_NUM_VALUE_H
#include "Value.h"

struct NOVASCRIPT_API NovaFloat : public NovaValue {
	NovaFloat(float num) : novanum(num) {};
	NovaFloat(float* cppnum) : cppnum(cppnum) { is_cpp = true; };

	float novanum = 0;
	bool is_cpp = false;
	float* cppnum = nullptr;

	float* Num();
	const float CNum() const;
	std::shared_ptr<NovaValue> Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override;
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) override;
	bool Assign(std::shared_ptr<NovaValue> rhs) override;

protected:

};


#endif
