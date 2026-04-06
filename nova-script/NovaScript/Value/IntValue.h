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
	NovaValue* Copy() override;
	std::string ToString() const override;
	std::string Type() const override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override;
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) override;
	NovaValue* Assign(NovaValue* rhs) override;

protected:

	void OnDestroy() override;

};

#endif
