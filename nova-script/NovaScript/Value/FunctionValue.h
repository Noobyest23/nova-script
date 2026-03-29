#ifndef NOVASCRIPT_FUNCTION_VALUE_H
#define NOVASCRIPT_FUNCTION_VALUE_H
#include "Value.h"
#include <vector>
#include "../ASTNodes/StmtNode.h"

class Interpretor;

struct NovaFunction : public NovaValue {
	NovaFunction(FuncDeclNode* node) : fn(node) {};
	NovaFunction(NovaValue* (*cppfn)(std::vector<NovaValue*>&)) : cppfn(cppfn) {};
	FuncDeclNode* fn = nullptr;
	NovaValue* (*cppfn)(std::vector<NovaValue*>&) = nullptr;

	NovaValue* Call(std::vector<NovaValue*> args, Interpretor* i) const;

	std::string Type() const override;
	std::string ToString() const override;
	NovaValue* Copy() const override;

};


#endif
