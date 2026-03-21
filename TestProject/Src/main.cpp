#include <NovaScript/NovaScript.h>
#include <NovaScript/Interpretor/Value.h>
#include <iostream>

struct Script {
	Script(std::string filepath) {
		interpretor = BuildScript(filepath.c_str());
	}

	void Execute() {
		ExecuteScript(interpretor);
	}

	Value* CallFunction(std::string func_name, std::vector<Value*> args) {
		ValueHandle handle = CallFunc(interpretor, func_name.c_str(), &args);
		return static_cast<Value*>(handle);
	}

	~Script() {
		if (interpretor) {
			DestroyScript(interpretor);
		}
	}

	InterpretorHandle interpretor = nullptr;
};

static bool stop = false;

static void PushErrorCallback(const char* msg, int sevarity) {
	switch (sevarity) {
	case 0:
		std::cout << msg << "\n";
		break;
	case 1:
		std::cout << "\033[33m" << msg << "\033[0m\n";
		break;
	case 2:
		std::cout << "\033[31m" << msg << "\033[0m\n";
		break;
	}
}

static void ExitCallback(const char* msg) {
	std::cout << msg;
	exit(0);
}

int main() {
	SetErrorCallback(PushErrorCallback);
	SetExitCallback(ExitCallback);
	for (int i = 0; i < 20000; i++) {
		Script script("test.ns");
		script.Execute();

		//int x = 10;

		//script.CallFunction("AddTen", { new Value(CPPVariable(x)) });
		//std::cout << x << "\n";
	}

	std::cout << "Script Finished Execution\n";
	std::cin.get();
}
