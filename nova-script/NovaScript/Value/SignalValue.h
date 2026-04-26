#ifndef NOVASCRIPT_SIGNAL_H
#define NOVASCRIPT_SIGNAL_H
#include "Value.h"
#include "FunctionValue.h"
#include "ArrayValue.h"
#include "../Library/nova_std_macro.h"
#include "../NovaErrorPush.h"

struct NovaSignal : public NovaValue {
	NovaSignal() {};
	NovaSignal(std::vector<std::shared_ptr<NovaFunction>> connections) : connections(connections) {};

	std::shared_ptr<NovaValue> Copy() const override { return std::make_shared<NovaSignal>(connections); };
	std::string ToString() const override { return "Signal with " + std::to_string(connections.size()) + " connections"; };
	std::string Type() const override { return "Signal"; };
	bool Assign(std::shared_ptr<NovaValue>) { return false; };

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override { return nullptr; };
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) override { return false; };
	std::vector<std::shared_ptr<NovaFunction>> connections;

	std::unordered_map<std::string, std::shared_ptr<NovaValue>> GetFullAccessableList() override;

	static std::unordered_map<std::string, std::shared_ptr<NovaValue>> signal_accessables;

protected:

};

#endif