#ifndef NOVASCRIPT_SIGNAL_H
#define NOVASCRIPT_SIGNAL_H
#include "Value.h"
#include "FunctionValue.h"
#include "ArrayValue.h"
#include "../Library/nova_std_macro.h"
#include "../NovaErrorPush.h"

struct NovaSignal : public NovaValue {
	NovaSignal();
	NovaValue* Copy() override { return new NovaSignal(*this); };
	std::string ToString() const override { return "Signal with " + std::to_string(connections.size()) + " connections"; };
	std::string Type() const override { return "Signal"; };
	NovaValue* Assign(NovaValue* rhs) { return nullptr; };

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const { return nullptr; };
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) { return nullptr; };

	std::vector<NovaValue*> connections;

protected:

	void OnDestroy() override;

};

#endif