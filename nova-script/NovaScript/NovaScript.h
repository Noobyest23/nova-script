#ifndef NOVA_SCRIPT_DLL_API
#define NOVA_SCRIPT_DLL_API
#ifndef _API_INCLUDE
#define _API_INCLUDE

#ifdef NOVASCRIPT_EXPORTS
#define NOVASCRIPT_API __declspec(dllexport)
#else
#define NOVASCRIPT_API __declspec(dllimport)
#endif

#endif
#include <string>

extern "C" {
	// void PushError(const char* message, int sevarity);

	using InterpretorHandle = void*;
	using ValueHandle = void*;
	using ArgsHandle = void*;
	using ModuleHandle = void*;
	using ScopeHandle = void*;
	
	NOVASCRIPT_API void SetErrorCallback(void(*)(const char*, int));

	NOVASCRIPT_API void SetExitCallback(void(*)(const char*));

	NOVASCRIPT_API void SetProjectPath(void(*)(const char*));

	NOVASCRIPT_API InterpretorHandle BuildScript(const char* filepath);

	NOVASCRIPT_API void ExecuteScript(InterpretorHandle interpretor);

	NOVASCRIPT_API void DestroyScript(InterpretorHandle interpretor);

	NOVASCRIPT_API ValueHandle GetVariable(InterpretorHandle interpretor, const char* variable_name);

	NOVASCRIPT_API void PushVariable(InterpretorHandle interpretor, const char* variable_name, ValueHandle value);

	// The return value must be deleted manually
	NOVASCRIPT_API ValueHandle CallFunc(InterpretorHandle interpretor, const char* function_name, void* args);

	NOVASCRIPT_API ScopeHandle GetScope(InterpretorHandle interpretor);

	NOVASCRIPT_API void PushModule(InterpretorHandle interpretor, ModuleHandle mod);

	NOVASCRIPT_API void FlagUseConsole();

	NOVASCRIPT_API void PrintAST(const char* filepath);

	NOVASCRIPT_API const char* Version();


}

#endif