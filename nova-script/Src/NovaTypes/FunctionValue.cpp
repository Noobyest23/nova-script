#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Interpretor/Scope.h"

NovaValue* NovaFunction::Call(std::vector<NovaValue*> args) const {
	if (fn) {
		if (args.size() != fn->args.size()) {
			PushError("Argument size mismatch, function expected " + std::to_string(fn->args.size()) + " but got " + std::to_string(args.size()));
			return nullptr;
		}
		interpretor->PushScope();
		for (int i = 0; i < args.size(); i++) {
			interpretor->GetScopeAsObj()->Set(fn->args[i], args[i]);
		}
		for (StmtNode* node : fn->body) {
			if (ReturnStmtNode* ret = dynamic_cast<ReturnStmtNode*>(node)) {
				NovaValue* return_val = interpretor->EvaluateExpression(ret->return_value);
				return_val->AddRef();
				interpretor->PopScope();
				return return_val;
			}
			interpretor->EvaluateStatement(node);
		}
	}
	else if (cppfn) {
		return cppfn(args);
	}
	else {
		PushError("This function is null");
	}
	return nullptr;
}

std::string NovaFunction::ToString() const {
	if (fn) {
		return fn->Print();
	}
	else if (cppfn) {
		return "CPP side NovaFunction, Unable to print body";
	}
	else {
		PushError("This function is null");
	}
}

std::string NovaFunction::Type() const {
	return "NovaFunction";
}

NovaValue* NovaFunction::Copy() const {
	if (fn) {
		return new NovaFunction(fn, interpretor);
	}
	else if (cppfn) {
		return new NovaFunction(cppfn);
	}
	else {
		PushError("This function is null");
		return nullptr;
	}
}

NovaValue* NovaFunction::Assign(NovaValue* rhs) {
	// You cannot assign to a function
	return this;
}