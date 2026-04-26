#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/parser/Lexer.h"
#include "../NovaScript/parser/parser.h"
#include "../NovaScript/Library/nova_std.h"
#include "../NovaScript/ASTNodes/StmtNode.h"

#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Value/BoolValue.h"

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
	es(BreakPointNode*)
	es(ASTPrintNode*)
	es(ForEachNode*)
	es(WhileNode*)
	es(ExprAsStmt*)
	PushError("Unrecognized Statement", node);
}

es_decl(VarDeclNode* node) {
	// Add a variable to the scope
	std::shared_ptr<NovaValue> value = nullptr;
	if (node->right) {
		if (VariableNode* v = dynamic_cast<VariableNode*>(node->right)) {
			if (!v->as_ptr) {
				value = scope->Get(v->identifier)->Copy();
				scope->Set(node->identifier, value);
				return;
			}
			else {
				value = scope->Get(v->identifier);
				scope->Set(node->identifier, value);

				return;
			}
		}
	}
	value = EvaluateExpression(node->right);
	scope->LimitedSet(node->identifier, value);
}

es_decl(FuncDeclNode* node) {
	std::shared_ptr<NovaValue> func = std::make_shared<NovaFunction>(node, this);
	scope->Set(node->func_id, func);
}

es_decl(IfStmtNode* node) {
	std::shared_ptr<NovaValue> value = EvaluateExpression(node->expression);
	
	if (!value) { PushError("Conditional in if statement was null", node); return; };

	if (value->Type() == "Bool") {
		NovaBool* nbool = static_cast<NovaBool*>(value.get());
		PushScope();
		if (nbool->CB()) {
			for (StmtNode* stmt : node->body) {
				EvaluateStatement(stmt);
			}
		}
		else {
			for (StmtNode* stmt : node->else_body) {
				EvaluateStatement(stmt);
			}
		}
		PopScope();
	}
	else {
		PushError("Cannot perform if on an expression that does not evaluate to a boolean", node);
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
	std::shared_ptr<NovaValue> file = EvaluateExpression(node->file_path);
	if (!file) { PushError("Filepath in include statement evaluated to null", node); return; }

	if (file->Type() == "String") {
		if (file->ToString().ends_with(".ns")) {
			// Include another nova script file
			Interpretor i(file->ToString());
			if (node->as) {
				if (VariableNode* as = dynamic_cast<VariableNode*>(node->as)) {
					std::shared_ptr<NovaObject> object = std::make_shared<NovaObject>();
					for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : i.GetScopeAsObj()->variables) {
						object->PushBack(pair.first, pair.second);
					}
					object->PushBack("_NOVA_THIS", nullptr);
					scope->Set(as->identifier, object);
				}
				PushError("as in include statement is not a variable", node);
			}
			else {
				for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : i.GetScopeAsObj()->variables) {
					scope->Set(pair.first, pair.second);
				}
			}
		}
		else {
			// include a cpp module
			if (modules.find(file->ToString()) != modules.end()) {
				std::shared_ptr<NovaObject> obj = modules[file->ToString()]->GetModule();
				if (node->as) {
					if (VariableNode* as = dynamic_cast<VariableNode*>(node->as)) {
						obj->PushBack("_NOVA_THIS", nullptr);
						scope->Set(as->identifier, obj);
						return;
					}
					PushError("as in include statement is not a variable", node);
				}
				else {
					for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : *obj->accessables) {
						scope->Set(pair.first, pair.second);
					}
					return;
				}
			}
			else {
				PushError("Module '" + file->ToString() + "' does not exist", node);
			}
		}
	}
	else {
		PushError("Filepath in include must be a string", node);
		return;
	}

}

es_decl(BreakPointNode* node) {
	if (program) {
		Callbacker::PushError("[NovaScript] Breakpoint Hit!", 2);
		if (node->stmt) {
			Callbacker::PushError(("[NovaScript] AST of this statement: \n" + node->stmt->Print()).c_str(), 1);
		}
		Callbacker::PushError("[NovaScript] Current variables in scope:", 1);
		Callbacker::PushError(scope->Print().c_str(), 1);
	}
	if (node->stmt) {
		EvaluateStatement(node->stmt);
	}
}

es_decl(ASTPrintNode* node) {
	if (program) {
		Callbacker::PushError("[NovaScript] Super Breakpoint Hit!", 2);
		Callbacker::PushError(("[NovaScript] AST of this script: \n" + program->Print()).c_str(), 1);
	}
}

es_decl(ForEachNode* node) {
	if (VariableNode* var = dynamic_cast<VariableNode*>(node->variable)) {
		std::shared_ptr<NovaValue> val = EvaluateExpression(node->container);
		if (val->Type() == "Array") {
			NovaArray* arr = static_cast<NovaArray*>(val.get());
			PushScope();			

			for (int i = 0; i < arr->CArr().size(); i++) {
				if (var->as_ptr) {
					std::shared_ptr<NovaValue> v = arr->CArr()[i];
					scope->variables[var->identifier] = v;
				}
				else {
					std::shared_ptr<NovaValue> v = arr->CArr()[i]->Copy();
					scope->variables[var->identifier] = v;
				}

				for (StmtNode* stmt : node->body) {
					EvaluateStatement(stmt);
				}

			}

			PopScope();
		}
		else {
			PushError("Type in for each loop is not a container. Type is " + val->Type(), node);
		}
	}
	else {
		PushError("Variable initializer in for loop is not a variable", node);
	}
}

es_decl(WhileNode* node) {
	std::shared_ptr<NovaValue> condition = EvaluateExpression(node->expression);
	if (condition->Type() != "Bool") {
		PushError("While loop condition is not a boolean", node);
		return;
	}

	NovaBool* nb = static_cast<NovaBool*>(condition.get());

	while (nb->CB()) {
		for (StmtNode* stmt : node->body) {
			EvaluateStatement(stmt);
		}

		condition = EvaluateExpression(node->expression);
		if (condition->Type() == "Bool") {
			nb = static_cast<NovaBool*>(condition.get());
		}
		else {
			PushError("Condition is no longer a boolean value", node);
			break;
		}
	}

}

es_decl(ExprAsStmt* node) {
	EvaluateExpression(node->expr);
}