#ifndef NOVASCRIPT_NULL_VALUE_H
#define NOVASCRIPT_NULL_VALUE_H
#include "Value.h"

struct NovaNull : public NovaValue {

	NovaValue* Copy() override {
		return this;
	};
	std::string ToString() const override {
		return "null";
	};
	std::string Type() const override {
		return "null";
	};
	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const override {
		OpFailed(rhs, op);  return nullptr;
	};
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) override {
		OpFailed(rhs, op);  return nullptr;
	};
	NovaValue* Assign(NovaValue* rhs) override {
		return nullptr;
	};

protected:

	void OnDestroy() override {
		AddRef();
	};

};


#endif
