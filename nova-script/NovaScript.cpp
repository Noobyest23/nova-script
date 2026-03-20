#include "NovaScript.h"
#include "NovaErrorPush.h"

#include "NovaScript/Interpretor/Interpretor.h"
#include "pch.h"
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
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		interpretor->Exec();
	}

	void DestroyScript(InterpretorHandle interpretor) {
		Interpretor* i = static_cast<Interpretor*>(interpretor);
		delete i;
	}

	ValueHandle GetVariable(InterpretorHandle i, const char* variable_name) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		return interpretor->Get(variable_name);
	}

	void PushVariable(InterpretorHandle i, const char* val_name, ValueHandle val) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		Value* value = static_cast<Value*>(val);
		interpretor->Set(val_name, *value);
	}

	ValueHandle CallFunc(InterpretorHandle i, const char* function_name, void* a) {
		Interpretor* interpretor = static_cast<Interpretor*>(i);
		std::vector<Value*>* args = static_cast<std::vector<Value*>*>(a);
		return new Value(interpretor->Call(function_name, *args));
	}

	ArgsHandle CreateArgs() {
		return new std::vector<Value*>;
	}

	void PushArg(ArgsHandle args, ValueHandle arg) {
		Value* value = static_cast<Value*>(arg);
		std::vector<Value*>* vec = static_cast<std::vector<Value*>*>(args);
		vec->push_back(value);
	}

	void DeleteArgs(ArgsHandle args) {
		delete args;
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

}
