#include "Interpretor.h"
#include "Scope.h"
#include "Value.h"
#include "../ASTNodes/ProgramNode.h"
#include "../Parser/Lexer.h"
#include "../Parser/Parser.h"
#include "../Library/nova_std.h"
#include "../../NovaErrorPush.h"
#include "../../pch.h"
#include "../Parser/Optimizer.h"

Interpretor::Interpretor(const std::string& filepath) {
	Lexer lexer(filepath.c_str());
	auto tokens = lexer.Parse();
	Parser parser(tokens);
	program = parser.Parse();
	Optimizer optimizer(program);
	program = optimizer.Optimize();

	Init();
}

Interpretor::~Interpretor() {
	if (program) {
		program->Delete();
	}
	while (scope) { 
		PopScope(); 
	}
	for (std::pair<std::string, NovaModule*> pair : modules) {
		delete pair.second;
	}

	modules.clear();
}

void Interpretor::Init() {
	scope = new Scope();
	if (!program) {
		return;
	}
	if (not program->valid_program) {
		PushError("Parser errors detected, cannot initialize script");
		return;
	}
	
	modules["string"] = new NovaStringModule;
	modules["io"] = new NovaIOModule;
	modules["vector"] = new NovaVectorModule;
	modules["math"] = new NovaMathModule;
}

void Interpretor::Exec() {
	if (!program) {
		return;
	}
	if (not program->valid_program) {
		PushError("Parser errors detected, cannot execute script");
		return;
	}
	for (StmtNode* stmt : program->Statements) {
		EvaluateStatement(stmt);
	}
}

void Interpretor::PushError(const std::string& message) {
	Callbacker::PushError(message.c_str(), 2);
};

void Interpretor::Set(const std::string& var_name, const Value& value) {
	scope->Set(var_name, value);
}

Value Interpretor::Call(const std::string& func_name, std::vector<Value*>& args) {
	Value* func = scope->Get(func_name);
	if (!func) {
		return Value();
	}
	if (func->data.index() == 8) { // cpp function
		CPPFunction f = std::get<CPPFunction>(func->data);
		return f(args);
	}
	else { // nova function
		NovaFunction f = std::get<NovaFunction>(func->data);
		Scope* s = new Scope(scope);
		scope = s;
		for (int i = 0; i < f->args.size(); i++) {
			s->Set(f->args[i], *args[i]);
		}

		for (StmtNode* stmt : f->body) {
			EvaluateStatement(stmt);
			if (return_flag) {
				for (Value* arg : args) {
					arg->Release();
				}
				return_flag = false;
				PopScope();
				return return_val;
			}
		}

		for (Value* arg : args) {
			arg->Release();
		}

		PopScope();
		return nullval;
	}
}

void Interpretor::PushScope() {
	Scope* n_scope = new Scope(scope);
	scope = n_scope;
}

void Interpretor::PopScope() {
	if (scope->parent) {
		Scope* parent = scope->parent;
		delete scope;
		scope = parent;
	}
	else {
		delete scope;
		scope = nullptr;
	}
}

Value* Interpretor::Get(const std::string& var_name) {
	return scope->Get(var_name);
}

Scope* Interpretor::GetScopeAsObj() {
	return scope;
}
#include "../Library/NovaModule.h"
void Interpretor::PushModule(NovaModule* mod) {
	modules[mod->module_name] = mod;
}