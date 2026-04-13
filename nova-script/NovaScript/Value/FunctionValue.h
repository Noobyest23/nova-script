#ifndef NOVASCRIPT_FUNCTION_VALUE_H
#define NOVASCRIPT_FUNCTION_VALUE_H
#include "Value.h"
#include <vector>
#include "../ASTNodes/StmtNode.h"

class Interpretor;

struct NOVASCRIPT_API NovaFunction : public NovaValue {
	NovaFunction(FuncDeclNode* node, Interpretor* interpretor, bool this_qualified = false) : fn(node), interpretor(interpretor), this_qualified(this_qualified) {};
	NovaFunction(NovaValue* (*cppfn)(std::vector<NovaValue*>&), bool this_qualified = false) : cppfn(cppfn), this_qualified(this_qualified) {};
	FuncDeclNode* fn = nullptr;
	NovaValue* (*cppfn)(std::vector<NovaValue*>&) = nullptr;
	Interpretor* interpretor = nullptr;
	bool this_qualified;

	NovaValue* Call(std::vector<NovaValue*> args) const;

	std::string Type() const override;
	std::string ToString() const override;
	NovaValue* Copy() override;
	NovaValue* Assign(NovaValue* rhs) override;

	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const { return nullptr; };
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) { return nullptr; };

protected:

	void OnDestroy() override;

};


#endif
