#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Interpretor/Scope.h"
#include "../NovaScript/Value/Value.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/ASTNodes/ProgramNode.h"
#include "../NovaScript/Parser/Lexer.h"
#include "../NovaScript/Parser/Parser.h"
#include "../NovaScript/Library/nova_std.h"
#include "../NovaScript/NovaErrorPush.h"
#include "../NovaScript/Parser/Optimizer.h"

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
	
	modules["io"] = new NovaIOModule;
	modules["vector"] = new NovaVectorModule;
	modules["functional"] = new NovaFunctionalModule;
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
		PurgeStack();
	}
}

void Interpretor::PushError(const std::string& message, ASTNode* current_node) {
	std::string msg = message + (current_node ? " : " + current_node->Print() : "");
	Callbacker::PushError(msg.c_str(), 2);
};

void Interpretor::Set(const std::string& var_name, NovaValue* value) {
	scope->Set(var_name, value);
}

NovaValue* Interpretor::Call(const std::string& func_name, std::vector<NovaValue*>& args) {
	NovaValue* v = scope->Get(func_name);
	if (v) {
		if (v->Type() == "NovaFunction") {
			NovaFunction* fn = static_cast<NovaFunction*>(v);
			NovaValue* result = fn->Call(args);
			return result;
		}
		else {
			PushError("Cannot use call a type of " + v->Type());
		}
	}
	else {
		PushError("Cannot call function " + func_name + " the function either doesnt exist in scope or is null");
	}
	return nullptr;
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

NovaValue* Interpretor::Get(const std::string& var_name) {
	NovaValue* v = scope->Get(var_name);
	v->AddRef();
	return v;
}

Scope* Interpretor::GetScopeAsObj() {
	return scope;
}

#include "../NovaScript/Library/NovaModule.h"
void Interpretor::PushModule(NovaModule* mod) {
	modules[mod->module_name] = mod;
}

void Interpretor::PurgeStack() {
	for (NovaValue* lit : literal_stack) {
		if (lit) {
			lit->Release();
		}
		literal_stack.clear();
	}
}