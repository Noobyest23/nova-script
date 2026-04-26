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

std::shared_ptr<NovaNull> Interpretor::null = std::make_shared<NovaNull>();

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
	modules["math"] = new NovaMathModule;
	modules["os"] = new NovaOSModule;
	modules["global"] = new NovaGlobalModule;
	std::shared_ptr<NovaObject> globals = modules["global"]->GetModule();
	for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : (*globals->accessables)) {
		scope->Set(pair.first, pair.second);
	}
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
		if (should_stop) {
			break;
		}
		EvaluateStatement(stmt);
		PurgeStack();
	}
}

void Interpretor::PushError(const std::string& message, ASTNode* current_node) {
	std::string msg = "Runtime Error: " + message + "\n";
	if (current_node) {
		msg += current_node->Print() + " At line " + std::to_string(current_node->line) + ", Column " + std::to_string(current_node->column) + "\n";
		msg += "^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
	}
	msg += "Quitting after error";
	should_stop = true;
	Callbacker::PushError(msg.c_str(), 2);
};

void Interpretor::Set(const std::string& var_name, std::shared_ptr<NovaValue> value) {
	scope->Set(var_name, value);
}

std::shared_ptr<NovaValue> Interpretor::Call(const std::string& func_name, std::vector<std::shared_ptr<NovaValue>>& args) {
	std::shared_ptr<NovaValue> v = scope->Get(func_name);
	if (v) {
		if (v->Type() == "Function") {
			NovaFunction* fn = static_cast<NovaFunction*>(v.get());
			std::shared_ptr<NovaValue> result = fn->Call(args);
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

std::shared_ptr<NovaValue> Interpretor::Get(const std::string& var_name) {
	std::shared_ptr<NovaValue> v = scope->Get(var_name);
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
	for (std::shared_ptr<NovaValue> lit : literal_stack) {
		literal_stack.clear();
	}
}