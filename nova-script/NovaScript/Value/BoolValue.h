#ifndef NOVASCRIPT_VALUE_BOOL_H
#define NOVASCRIPT_VALUE_BOOL_H

#include "Value.h"

struct NOVASCRIPT_API NovaBool : public NovaValue {
	NovaBool(bool b) : novab(b) {};
	NovaBool(bool* cppb) : cppb(cppb) {};

	bool novab = false;
	bool* cppb = nullptr;

	bool* B();
	const bool& CB() const;

	std::shared_ptr<NovaValue> Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override;
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) { return false; };
	bool Assign(std::shared_ptr<NovaValue> rhs) override;

protected:

};

#endif