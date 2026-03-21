#include "../../nova-script/NovaScript.h"
#include <iostream>

struct Script {
	Script(std::string filepath) {
		interpretor = BuildScript(filepath.c_str());
	}

	void Execute() {
		ExecuteScript(interpretor);
	}

	~Script() {
		if (interpretor) {
			DestroyScript(interpretor);
		}
	}

	void* interpretor = nullptr;
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

	Script script("test.ns");
	script.Execute();

	std::cout << "Hello C++!\n";

	while (!stop) {

	}
}
