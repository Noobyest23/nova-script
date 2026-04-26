#include "NovaScript/NovaScript.h"
#include "NovaScript/NovaErrorPush.h"
#include "NovaScript/Value/Value.h"
#include "NovaScript/Interpretor/Interpretor.h"
#include "NovaScript/NovaErrorPush.h"

extern "C" {

	void SetErrorCallback(void(*function)(const char*, int)) {
		Callbacker::_PushErrorCallback = function;
	}

	void SetExitCallback(void(*function)(const char*)) {
		Callbacker::_ExitCallback = function;
	}

	void SetProjectPath(void(*function)(const char*)) {
		Callbacker::_ProjectPathSetCallback = function;
	}

	InterpretorHandle BuildScript(const char* file_path) {
		return new Interpretor(file_path);
	}

	void ExecuteScript(InterpretorHandle i) {
		try {
			Interpretor* interpretor = static_cast<Interpretor*>(i);
			interpretor->Exec();
		}
		catch (std::exception e) {
			Callbacker::PushError("[NovaScript] Fatal Error while executing script!", 2);
			Callbacker::PushError(e.what(), 0);
		}
	}

	void DestroyScript(InterpretorHandle interpretor) {
		Interpretor* i = static_cast<Interpretor*>(interpretor);
		delete i;
	}

	ValueHandle GetVariable(InterpretorHandle i, const char* variable_name) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		return interpretor->Get(variable_name).get();
	}

	void PushVariable(InterpretorHandle i, const char* val_name, ValueHandle val) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		NovaValue* value = static_cast<NovaValue*>(val);
		interpretor->Set(val_name, value->Copy());
	}

	ValueHandle CallFunc(InterpretorHandle i, const char* function_name, void* a) {
		try {
			Interpretor* interpretor = static_cast<Interpretor*>(i);
			std::vector<NovaValue*>* args = static_cast<std::vector<NovaValue*>*>(a);
			std::vector<std::shared_ptr<NovaValue>> new_args;
			for (NovaValue* arg : *(args)) {
				new_args.push_back(arg->Copy());
			}
			
			NovaValue* result = interpretor->Call(function_name, new_args).get();
			interpretor->PurgeStack();
			return result;
		}
		catch (std::exception e) {
			Callbacker::PushError("[NovaScript] Fatal Error while calling function!", 2);
			Callbacker::PushError(e.what(), 0);
		}
	}

	void PushModule(InterpretorHandle i, ModuleHandle m) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		NovaModule* mod = static_cast<NovaModule*>(m);
		interpretor->PushModule(mod);
	}

	void FlagUseConsole() {
		_use_console = true;
	}
	
	ScopeHandle GetScope(InterpretorHandle i) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		return interpretor->GetScopeAsObj();
	}

#include "NovaScript/Parser/Parser.h"
#include "NovaScript/Parser/Lexer.h"
#include "NovaScript/ASTNodes/ASTNode.h"

	void PrintAST(const char* filepath) {
		Lexer lexer(filepath);
		auto tokens = lexer.Parse();
		Parser parser(tokens);
		ProgramNode* node = parser.Parse();
		Callbacker::PushError(node->Print(), 0);
		node->Delete();
	}

	const char* Version() { return "v0.4b"; };

	const char* Changelog() { return ""; };

}
