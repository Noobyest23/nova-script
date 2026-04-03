#ifndef NOVASCRIPT_SIGNAL_H
#define NOVASCRIPT_SIGNAL_H
#include "Value.h"
#include "FunctionValue.h"
#include "ArrayValue.h"
#include "../Library/nova_std_macro.h"
#include "../NovaErrorPush.h"

struct NovaSignal : public NovaValue {
	NovaSignal();
	NovaValue* Copy() const override { return new NovaSignal(*this); };
	std::string ToString() const override { return "Signal with " + std::to_string(connections.size()) + " connections"; };
	std::string Type() const override { return "Signal"; };
	NovaValue* Assign(NovaValue* rhs) { return nullptr; };

	std::vector<NovaValue*> connections;

};

#endif