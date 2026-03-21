#ifndef NOVASCRIPT_INTERPRETOR_H
#define NOVASCRIPT_INTERPRETOR_H

#include "Value.h"
#include "../ASTNodes/ProgramNode.h"

class NovaModule;
struct Value;
struct Scope;

#include <string>
#include <unordered_map>

class Interpretor {
public:
	Interpretor(ProgramNode* program) : program(program) { Init(); };
	Interpretor(const std::string& filepath);
	~Interpretor();

	// Initializes the global scope
	void Init();

	// Executes the body of the script
	void Exec();

	// Calls a Function from nova script
	Value Call(const std::string& func_name, std::vector<Value*>& args);

	Value* Get(const std::string& var_name);

	void Set(const std::string& var_name, const Value& var);

	Scope* GetScopeAsObj();

	void PushScope();
	void PopScope();

	void EvaluateStatement(StmtNode*);
	Value EvaluateExpression(ExprNode*);

	void PushModule(NovaModule* mod);

private:

	#pragma region Evaluate Statement Overloads
	
	void EvaluateStatement(VarDeclNode*);
	void EvaluateStatement(FuncDeclNode*);
	void EvaluateStatement(IfStmtNode*);
	void EvaluateStatement(TypeDeclNode*);
	void EvaluateStatement(IncludeNode*);
	void EvaluateStatement(ReturnStmtNode*);
	void EvaluateStatement(BreakPointNode*);
	void EvaluateStatement(ASTPrintNode*);
	void EvaluateStatement(ForEachNode*);
	void EvaluateStatement(WhileNode*);
	void EvaluateStatement(ExprAsStmt*);
	#pragma endregion
	#pragma region Evaluate Expression Overloads
	
	Value EvaluateExpression(VariableNode*);
	Value EvaluateExpression(OpNode*);
	Value EvaluateExpression(FuncCallNode*);
	Value EvaluateExpression(TernaryNode*);
	Value EvaluateExpression(IntLiteralNode*);
	Value EvaluateExpression(FloatLiteralNode*);
	Value EvaluateExpression(BoolLiteralNode*);
	Value EvaluateExpression(StringLiteralNode*);
	Value EvaluateExpression(ArrayLiteralNode*);
	Value EvaluateExpression(DotAccessNode*);
	Value EvaluateExpression(Vector2LiteralNode*);
	Value EvaluateExpression(Vector3LiteralNode*);
	Value EvaluateExpression(Vector4LiteralNode*);
	Value EvaluateExpression(ArrayAccessNode*);
	Value EvaluateExpression(CompoundOp*);
	Value EvaluateExpression(AssignmentNode*);
	Value EvaluateExpression(NullLiteralNode*);
	Value EvaluateExpression(NotNode*);

	Value* EvaluateExpressionPtr(ExprNode*);
	Value* EvaluateExpressionPtr(VariableNode*);
	Value* EvaluateExpressionPtr(TernaryNode*);
	Value* EvaluateExpressionPtr(DotAccessNode*);
	Value* EvaluateExpressionPtr(ArrayAccessNode*);

	#pragma endregion

	ProgramNode* program;
	Scope* scope = nullptr;

	void PushError(const std::string& message);

	bool return_flag = false;
	Value return_val = Value();
	Value* return_val_ptr = nullptr;

	Value nullval = Value();

	std::unordered_map<std::string, NovaModule*> modules;
	std::unordered_map<TypeDeclNode*, FuncDeclNode*> nova_types;
};

// Todo
// Type Declare
// Scene Tree
// Extendable Objects
// signal support
// std lib stuff
// vector
// array

#endif