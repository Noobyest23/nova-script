#ifndef NOVASCRIPT_NULL_VALUE_H
#define NOVASCRIPT_NULL_VALUE_H
#include "Value.h"

struct NovaNull : public NovaValue {

	std::shared_ptr<NovaValue> Copy() const override;
	std::string ToString() const override {
		return "null";
	};
	std::string Type() const override {
		return "null";
	};
	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override {
		return nullptr;
	};
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) override {
		return false;
	};
	bool Assign(std::shared_ptr<NovaValue> rhs) override {
		return false;
	};

protected:


};


#endif
