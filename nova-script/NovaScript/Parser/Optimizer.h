#ifndef NOVA_SCRIPT_OPTIMIZER
#define NOVA_SCRIPT_OPTIMIZER

class Interpretor;
struct ProgramNode;
struct StmtNode;
struct ExprNode;
class Scope;
class Value;

#include <unordered_map>
#include <string>


class Optimizer {
public:

	Optimizer(ProgramNode* program) : old_program(program) {};
	ProgramNode* Optimize();

private:

	StmtNode* OptimizeStatement(StmtNode*) { return nullptr; };
	ExprNode* OptimizeExpression(ExprNode*) { return nullptr; };

	ProgramNode* old_program;

	std::vector<StmtNode*> block_stack;
	bool emplace_block_flag = false;

	Interpretor* const_evaluator = nullptr;

	ExprNode* CreateLiteralFromValue(Value* value) { return nullptr; };
};

#endif