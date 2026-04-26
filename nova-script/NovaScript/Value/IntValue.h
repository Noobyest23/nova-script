#ifndef NOVASCRIPT_INT_VALUE_H
#define NOVASCRIPT_INT_VALUE_H
#include "Value.h"

struct NOVASCRIPT_API NovaInt : public NovaValue {
	NovaInt(int num) : novanum(num) {};
	NovaInt(int* cppnum) : cppnum(cppnum) { is_cpp = true; };
	int novanum = 0;
	bool is_cpp = false;
	int* cppnum = nullptr;
	int* Num();
	const int CNum() const;
	std::shared_ptr<NovaValue> Copy() const override;
	std::shared_ptr<NovaValue> CopyPtr() override;
	std::string ToString() const override;
	std::string Type() const override;

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override;
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) override;
	bool Assign(std::shared_ptr<NovaValue> rhs) override;

};

#endif
