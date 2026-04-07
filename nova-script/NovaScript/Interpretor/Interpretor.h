#ifndef NOVASCRIPT_INTERPRETOR_H
#define NOVASCRIPT_INTERPRETOR_H
#include "../ASTNodes/ProgramNode.h"
#include "../Value/NullValue.h"

class NovaModule;
struct Scope;
#include <string>
#include <unordered_map>

#define ee(type) NovaValue* EvaluateExpression(type);

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
	NovaValue* Call(const std::string& func_name, std::vector<NovaValue*>& args);

	NovaValue* Get(const std::string& var_name);

	void Set(const std::string& var_name, NovaValue* var);

	Scope* GetScopeAsObj();

	void PushScope();
	void PopScope();

	void EvaluateStatement(StmtNode*);
	NovaValue* EvaluateExpression(ExprNode*);

	void PushModule(NovaModule* mod);
	void PurgeStack();
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
	ee(VariableNode*);
	ee(OpNode*);
	ee(FuncCallNode*);
	ee(TernaryNode*);
	ee(IntLiteralNode*);
	ee(FloatLiteralNode*);
	ee(BoolLiteralNode*);
	ee(StringLiteralNode*);
	ee(ArrayLiteralNode*);
	ee(DotAccessNode*);
	ee(ArrayAccessNode*);
	ee(AssignmentNode*);
	ee(CompoundOp*);
	ee(NullLiteralNode*);
	ee(NotNode*);
	ee(IsNode*);
	#pragma endregion

	ProgramNode* program;
	Scope* scope = nullptr;

	void PushError(const std::string& message, ASTNode* current_node = nullptr);
	
	NovaNull* null = new NovaNull();

	std::unordered_map<std::string, NovaModule*> modules;
	std::unordered_map<TypeDeclNode*, FuncDeclNode*> nova_types;
	std::vector<NovaValue*> literal_stack;
};


#endif