#include "Interpretor.h"
#include "../parser/Lexer.h"
#include "../parser/parser.h"
#include "../Library/nova_std.h"
#include "../ASTNodes/StmtNode.h"

#define es_decl(type) void Interpretor::EvaluateStatement(type)
#define es(type) if (type n = dynamic_cast<type>(node)) { EvaluateStatement(n); return; }

#define STR(type) #type



es_decl(StmtNode* node) {
	if (!node) {
		PushError("Statement was null");
		return;
	}
	es(VarDeclNode*)
	es(FuncDeclNode*)
	es(IfStmtNode*)
	es(TypeDeclNode*)
	es(IncludeNode*)
	es(ReturnStmtNode*)
	es(BreakPointNode*)
	es(ASTPrintNode*)
	es(ForEachNode*)
	es(WhileNode*)
	es(ExprAsStmt*)
	PushError("Unrecognized Statement " + node->Print());
}

es_decl(VarDeclNode* node) {
	// Add a variable to the scope
	Value value;
	if (node->right) {
		value = EvaluateExpression(node->right);
	}

	scope->Set(node->identifier, value);
}

es_decl(FuncDeclNode* node) {
	Value var(node);
	scope->Set(node->func_id, var);
}

es_decl(IfStmtNode* node) {
	Value value = EvaluateExpression(node->expression);
	
	if (value.IsBool()) {
		if (value.GetBool()) {
			for (StmtNode* stmt : node->body) {
				EvaluateStatement(stmt);
			}
		}
		else {
			for (StmtNode* stmt : node->else_body) {
				EvaluateStatement(stmt);
			}
		}
	}
	else {
		// fallback to a boolean op
		OpNode* op = new OpNode(node->expression, nullptr, "+");
		Value val = EvaluateExpression(op);
		if (val.IsBool()) {
			if (val.GetBool()) {
				for (StmtNode* stmt : node->body) {
					EvaluateStatement(stmt);
				}
			}
			else {
				for (StmtNode* stmt : node->else_body) {
					EvaluateStatement(stmt);
				}
			}
		}
		else {
			PushError("Cannot perform if on an expression that does not evaluate to a boolean");
		}
		delete op;
	}
}

es_decl(TypeDeclNode* node) {
	FuncDeclNode* valid_constructor = nullptr;
	
	for (StmtNode* stmt : node->definition) {
		if (FuncDeclNode* constructor = dynamic_cast<FuncDeclNode*>(stmt)) {
			if (constructor->func_id == node->type_name) {
				valid_constructor = constructor;
			}
		}
	}

	nova_types[node] = valid_constructor;
}

es_decl(IncludeNode* node) {
	// Parse through the file
	Value val = EvaluateExpression(node->file_path);
	if (!val.IsString()) {
		PushError("Filepath must evaluate to string " + node->Print());
		return;
	}
	std::string filepath = val.GetString();
	if (!filepath.ends_with(".ns")) {
		Value val;
		auto it = modules.find(filepath);
		if (it != modules.end()) {
			NovaType type = modules[filepath]->GetModule();
			val = Value(type);
		}
		if (val.data.index() == 0) {
			PushError("Cannot include standard lib " + node->file_path->Print() + " because it can't be found");
			return;
		}

		if (VariableNode* var = dynamic_cast<VariableNode*>(node->as)) {
			scope->Set(var->identifier, val);
		}
		else {
			for (std::pair<std::string, Value> pair : std::get<Scope>(val.data).variables) {
				scope->Set(pair.first, pair.second);
			}
		}
	}
	else {
		filepath = Callbacker::_proj_path + filepath;
		Lexer lexer(filepath.c_str());
		ProgramNode* program = nullptr;
		{
			std::vector<Token> tokens = lexer.Parse();
			Parser parser(tokens);
			program = parser.Parse();
		}

		if (!program) {
			PushError("Error including file " + node->Print());
		}

		Interpretor interpretor(program);
		Scope* s = interpretor.GetScopeAsObj();

		
		if (VariableNode* var = dynamic_cast<VariableNode*>(node->as)) {
			scope->Set(var->identifier, val);
		}
		else {
			for (std::pair<std::string, Value> pair : s->variables) {
				scope->Set(pair.first, pair.second);
			}
		}
	}
}

es_decl(ReturnStmtNode* node) {
	return_flag = true;
	return_val = EvaluateExpression(node->return_value);
}

es_decl(BreakPointNode* node) {
	if (program) {
		Callbacker::PushError("[NovaScript] Breakpoint Hit!", 2);
		Callbacker::PushError(("[NovaScript] AST of this statement: " + node->stmt->Print()).c_str(), 1);
		Callbacker::PushError("[NovaScript] Current variables in scope: ", 1);
		Callbacker::PushError(scope->Print().c_str(), 1);
	}
	EvaluateStatement(node->stmt);
}

es_decl(ASTPrintNode* node) {
	if (program) {
		Callbacker::PushError("[NovaScript] Super Breakpoint Hit!", 2);
		Callbacker::PushError(("[NovaScript] AST of this script: " + program->Print()).c_str(), 1);
	}
}

es_decl(ForEachNode* node) {
	PushScope();
	Value container = EvaluateExpression(node->container);
	std::string var_name;
	if (node->variable) {
		if (VariableNode* var = dynamic_cast<VariableNode*>(node->variable)) {
			var_name = var->identifier;
		}
		else {
			PushError("For loop variable is not a variable");
			return;
		}
	}
	else {
		PushError("For loop variables is null");
		return;
	}
	if (!container.IsArray()) {
		PushError("For loop cannot iterate through " + container.Type());
		return;
	}
	
	std::vector<Value>& arr = container.GetArray();
	for (Value& val : arr) {
		scope->Set(var_name, val);
		for (StmtNode* stmt : node->body) {
			EvaluateStatement(stmt);
		}
	}
	PopScope();

}

es_decl(WhileNode* node) {
	Value condition = EvaluateExpression(node->expression);
	if (!condition.IsBool()) {
		PushError("While loop condition is not a boolean " + node->Print());
		return;
	}
	if (node->expression->constant) {
		PushError("While loop condition is constant " + node->Print());
		return;
	}

	while (condition.GetBool()) {
		for (StmtNode* stmt : node->body) {
			EvaluateStatement(stmt);
		}
		condition = EvaluateExpression(node->expression);
	}

}

es_decl(ExprAsStmt* node) {
	EvaluateExpression(node->expr);
}