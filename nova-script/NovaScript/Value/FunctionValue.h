#ifndef NOVASCRIPT_FUNCTION_VALUE_H
#define NOVASCRIPT_FUNCTION_VALUE_H
#include "Value.h"
#include <vector>
#include "../ASTNodes/StmtNode.h"

class Interpretor;

struct NOVASCRIPT_API NovaFunction : public NovaValue {
	NovaFunction(FuncDeclNode* node, Interpretor* interpretor, bool this_qualified = false) : fn(node), interpretor(interpretor), this_qualified(this_qualified) {};
	NovaFunction(std::shared_ptr<NovaValue>(*cppfn)(std::vector<std::shared_ptr<NovaValue>>&), bool this_qualified = false) : cppfn(cppfn), this_qualified(this_qualified) {};
	FuncDeclNode* fn = nullptr;
	std::shared_ptr<NovaValue>(*cppfn)(std::vector<std::shared_ptr<NovaValue>>&) = nullptr;
	Interpretor* interpretor = nullptr;
	bool this_qualified;

	std::shared_ptr<NovaValue> Call(std::vector<std::shared_ptr<NovaValue>> args) const;

	std::string Type() const override;
	std::string ToString() const override;
	std::shared_ptr<NovaValue> Copy() const override;
	bool Assign(std::shared_ptr<NovaValue> rhs) override;

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override { return nullptr; };
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) override { return false; };

protected:

};


#endif
