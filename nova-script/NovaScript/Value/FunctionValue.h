#ifndef NOVASCRIPT_FUNCTION_VALUE_H
#define NOVASCRIPT_FUNCTION_VALUE_H
#include "Value.h"
#include <vector>
#include "../ASTNodes/StmtNode.h"

class Interpretor;

struct NOVASCRIPT_API NovaFunction : public NovaValue {
	NovaFunction(FuncDeclNode* node, Interpretor* interpretor) : fn(node), interpretor(interpretor) {};
	NovaFunction(NovaValue* (*cppfn)(std::vector<NovaValue*>&)) : cppfn(cppfn) {};
	FuncDeclNode* fn = nullptr;
	NovaValue* (*cppfn)(std::vector<NovaValue*>&) = nullptr;
	Interpretor* interpretor = nullptr;

	NovaValue* Call(std::vector<NovaValue*> args) const;

	std::string Type() const override;
	std::string ToString() const override;
	NovaValue* Copy() const override;
	NovaValue* Assign(NovaValue* rhs) override;
};


#endif
