#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Interpretor/Scope.h"

std::shared_ptr<NovaValue> NovaFunction::Call(std::vector<std::shared_ptr<NovaValue>> args) const {
	if (fn) {
		if (args.size() != fn->args.size()) {
			PushError("Argument size mismatch, function expected " + std::to_string(fn->args.size()) + " but got " + std::to_string(args.size()));
			return nullptr;
		}
		interpretor->PushScope();
		for (int i = 0; i < args.size(); i++) {
			interpretor->GetScopeAsObj()->LimitedSet(fn->args[i], args[i]);
		}
		for (StmtNode* node : fn->body) {
			if (interpretor->should_stop) {
				break;
			}
			if (ReturnStmtNode* ret = dynamic_cast<ReturnStmtNode*>(node)) {
				std::shared_ptr<NovaValue> return_val = interpretor->EvaluateExpression(ret->return_value);
				interpretor->PopScope();
				return return_val;
			}
			interpretor->EvaluateStatement(node);
		}
		interpretor->PopScope();
	}
	else if (cppfn) {
		return cppfn(args);
	}
	return nullptr;
}

std::string NovaFunction::ToString() const {
	if (fn) {
		return fn->Print();
	}

	return "CPP side NovaFunction, Unable to print body";
	
}
std::string NovaFunction::Type() const {
	return "Function";
}

std::shared_ptr<NovaValue> NovaFunction::Copy() const {
	if (fn) {
		return std::make_shared<NovaFunction>(fn, interpretor, this_qualified);
	}
	else return std::make_shared<NovaFunction>(cppfn);
}

bool NovaFunction::Assign(std::shared_ptr<NovaValue> rhs) {
	return true;
}