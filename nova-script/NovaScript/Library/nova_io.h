#ifndef NOVASCRIPT_IO_H
#define NOVASCRIPT_IO_H
#include <string>
#include "../NovaScript/Value/StringValue.h"
#include "../NovaErrorPush.h"
#include "../NovaScript.h"
#include "../NovaScript/Value/NovaObject.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "nova_std_macro.h"
#include "../Interpretor/Scope.h"
#include "NovaModule.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class NovaIOModule : public NovaModule {
public:

	NovaIOModule() {
		module_name = "io";
	}

	nova_std_decl(Print) {
		std::string output = "";
		for (NovaValue* arg : args) {
			output += arg ? arg->ToString() : "null"; if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 0);
		return null_value;
	}

	nova_std_decl(PrintWarning) {
		std::string output = "";
		for (NovaValue* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 1);
		return null_value;
	}

	nova_std_decl(PrintError) {
		std::string output = "";
		for (NovaValue* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 2);
		return null_value;
	}

	nova_std_decl(Input) {
		req_args(1);
		strget(string, 0);
		std::string in;
		std::cout << string->ToString() << " >: ";
		std::getline(std::cin, in);
		return new NovaString(in);
	}

	nova_std_decl(ReadFile) {
		req_args(1);
		strget(filePath, 0);

		std::ifstream file(filePath->CStr());
		if (!file.is_open()) {
			Callbacker::PushError("Could not open file for reading", 2);
			return null_value;
		}

		std::string content((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		file.close();
		return new NovaString(content);
	}

	nova_std_decl(WriteFile) {
		req_args(2); // path and content
		strget(filePath, 0);
		strget(content, 1);

		std::ofstream file(filePath->CStr());
		if (!file.is_open()) {
			Callbacker::PushError("Could not open file for writing", 2);
			return null_value;
		}

		file << content->CStr();
		file.close();

		return null_value;
	}

	nova_std_decl(RemoveFile) {
		req_args(1);
		strget(filepath, 0);
		try {
			if (fs::exists(filepath->CStr()) && fs::is_regular_file(filepath->CStr())) {
				fs::remove(filepath->CStr());
			}
			else {
				std::cerr << "Error: Path is not a file or does not exist." << std::endl;
				PushError("Error: Path is not a file or does not exist.");
			}
		}
		catch (const fs::filesystem_error& e) {
			PushError("Filesystem Error: " + std::string(e.what()) + "\n");
		}
	}

	nova_std_decl(RemoveDir) {
		req_args(1);
		strget(filepath, 0);

		try {
			if (fs::exists(filepath->CStr()) && fs::is_directory(filepath->CStr())) {
				fs::remove(filepath->CStr());
			}
			else {
				PushError("Error: Path is not a file or does not exist.");
			}
		}
		catch (const fs::filesystem_error& e) {
			PushError("Filesystem Error: " + std::string(e.what()) + "\n");
		}

	}


	NovaObject* GetModule() {
		NovaObject* obj = new NovaObject();
		obj->PushBack("Print", new NovaFunction(Print));
		obj->PushBack("PrintWarning", new NovaFunction(PrintWarning));
		obj->PushBack("PrintError", new NovaFunction(PrintError));
		obj->PushBack("Input", new NovaFunction(Input));
		objbindmethod(obj, ReadFile);
		objbindmethod(obj, WriteFile);
		objbindmethod(obj, RemoveFile);
		objbindmethod(obj, RemoveDir);
		return obj;
	}

protected:

	

};

#endif
