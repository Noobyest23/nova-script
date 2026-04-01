#include <../../../nova-script/nova-script/NovaScript/NovaScript.h>
#include <../../../nova-script/nova-script/NovaScript/Value/Value.h>
#include <../../../nova-script/nova-script/NovaScript/Value/IntValue.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>

struct Script {
	Script(std::string filepath) {
		interpretor = BuildScript(filepath.c_str());
	}

	void Execute() {
		ExecuteScript(interpretor);
	}

	NovaValue* CallFunction(std::string func_name, std::vector<NovaValue*> args) {
		ValueHandle handle = CallFunc(interpretor, func_name.c_str(), &args);
		return static_cast<NovaValue*>(handle);
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

void Help() {
	std::cout << "\n";
	std::cout << "- help - Prints all commands\n\n";
	std::cout << "- nova/script/filepath.ns - Runs a script\n\n";
	std::cout << "- parse - Builds a scripts program, and then prints what the interpretor reads it as\n\n";
	std::cout << "- make-test - Makes a test for this specific nova script file\n\n";
	std::cout << "- test - Tests a nova script file against its .ns.test files\n\n";
	std::cout << "- test-dir - Tests a directory of nova script files\n\n";
	std::cout << "\n";
}

std::string ReadFile(const std::string& path) {
	std::ifstream file(path);
	if (!file.is_open()) return "";
	return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void Test(const std::string& filepath) {
	bool passed = true;
	{
		std::stringstream buffer;
		std::streambuf* oldcout = std::cout.rdbuf(buffer.rdbuf());

		Script script(filepath);
		script.Execute();

		std::cout.rdbuf(oldcout);

		std::string currentOutput = buffer.str();
		std::string expectedOutput = ReadFile(filepath + ".test");

		if (currentOutput != expectedOutput) {
			std::cout << "[FAILED] Execution mismatch for: " << filepath << "\n";
			passed = false;
		}
	}

	{
		std::stringstream buffer;
		std::streambuf* oldcout = std::cout.rdbuf(buffer.rdbuf());

		PrintAST(filepath.c_str());

		std::cout.rdbuf(oldcout);

		std::string currentOutput = buffer.str();
		std::string expectedOutput = ReadFile(filepath + ".ast.test");

		if (currentOutput != expectedOutput) {
			std::cout << "[WARNING] AST mismatch for: " << filepath << "\n";
		}
	}

	if (passed) {
		std::cout << "[PASSED] " << filepath << "\n";
	}
	else {
		std::cout << "FAILED " << filepath << "\n";
	}

}

int main(int argc, char* argv[]) {
	SetErrorCallback(PushErrorCallback);
	SetExitCallback(ExitCallback);
	FlagUseConsole();
	if (argc == 1) {
		std::cout << "No arguments provided use 'help' to get started\n";
	}
	else {
		std::string arg1 = argv[1];
		if (arg1 == "help") {
			Help();
		}
		else if (arg1.ends_with(".ns")) {
			Script script(arg1);
			script.Execute();
		}
		else if (arg1 == "parse") {
			if (argc >= 3) PrintAST(argv[2]);
			else std::cout << "Missing filename for parse.\n";
		}
		else if (arg1 == "make-test") {
			// create a test for a script. Capture its script output, save to script.test. print its ast output to script.ast.test
			if (argc < 3) {
				std::cout << "Usage: make-test <filepath>\n";
			}
			else {
				std::string filepath = argv[2];

				{
					// 1. Capture Script Output
					std::ofstream outFile(filepath + ".test");
					std::streambuf* oldCout = std::cout.rdbuf(outFile.rdbuf()); // Redirect cout

					Script script(filepath);
					script.Execute();

					std::cout.rdbuf(oldCout); // Restore cout
					std::cout << "Test output saved to " << filepath << ".test\n";
				}
				{
					// 1. Capture Script Output
					std::ofstream outFile(filepath + ".ast.test");
					std::streambuf* oldCout = std::cout.rdbuf(outFile.rdbuf()); // Redirect cout

					PrintAST(filepath.c_str());

					std::cout.rdbuf(oldCout); // Restore cout
					std::cout << "Test output saved to " << filepath << ".test\n";
				}
			}
		}
		else if (arg1 == "test") {
			// run a test for a script. Compare its output to script.test. Compare its ast to script.ast.test (if its different this does mean that the test failed if the output is the same)
			if (argc < 3) std::cout << "Usage: test <filepath>\n";
			else Test(argv[2]);
		}
		else if (arg1 == "test-dir") {
			if (argc < 3) {
				std::cout << "Usage: test-dir <directory>\n";
			}
			else {
				for (const auto& entry : std::filesystem::directory_iterator(argv[2])) {
					if (entry.path().extension() == ".ns") {
						Test(entry.path().string());
					}
				}
			}
		}
		else {
			std::cout << "No correct arguments provided use 'help' to get started\n";
		}
		
	}
}



