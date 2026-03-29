#ifndef NOVASCRIPT_IO_H
#define NOVASCRIPT_IO_H

#ifdef _WIN32
#include <Windows.h>
#include <string>
#include "../NovaScript/Value/StringValue.h"
#include "../NovaErrorPush.h"
#include "../NovaScript.h"

struct InputDlgData {
	std::string* userInput = nullptr;
	std::string prompt;
};

static INT_PTR CALLBACK InputDlgProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		InputDlgData* data = reinterpret_cast<InputDlgData*>(lParam);
		SetWindowLongPtr(hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

		SetDlgItemTextA(hwndDlg, 1100, data->prompt.c_str());

		SetDlgItemTextA(hwndDlg, 1001, "");

		return TRUE;
	}

	case WM_COMMAND: {
		InputDlgData* data = reinterpret_cast<InputDlgData*>(GetWindowLongPtr(hwndDlg, GWLP_USERDATA));
		if (!data) return FALSE;

		if (LOWORD(wParam) == IDOK) {
			char buf[256] = {};
			GetDlgItemTextA(hwndDlg, 1001, buf, sizeof(buf));
			*data->userInput = buf;
			EndDialog(hwndDlg, IDOK);
			return TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL) {
			data->userInput->clear();
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	}
	return FALSE;
}
#endif
#include "nova_std_macro.h"
#include "../Interpretor/Scope.h"
#include "NovaModule.h"
#include <fstream>
#include <iostream>

class NovaIOModule : public NovaModule {
public:

	NovaIOModule() {
		module_name = "io";
	}

	nova_std_decl(Print) {
		std::string output = "[NovaScript] ";
		for (NovaValue* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 0);
		return null_value;
	}

	nova_std_decl(PrintWarning) {
		std::string output = "[NovaScript] ";
		for (NovaValue* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Callbacker::PushError(output.c_str(), 1);
		return null_value;
	}

	nova_std_decl(PrintError) {
		std::string output = "[NovaScript] ";
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
		if (_use_console) {
			std::cout << string->str << " >: ";
			std::getline(std::cin, in);
			return new NovaString(in);
		}
		else {
#ifdef _WIN32

			InputDlgData dlgData;
			dlgData.userInput = &in;
			dlgData.prompt = string->str;

			DialogBoxParamA(
				GetModuleHandle(nullptr),
				MAKEINTRESOURCEA(101),
				nullptr,
				InputDlgProc,
				reinterpret_cast<LPARAM>(&dlgData)
			);

			return new NovaString(in);
#endif
		}
	}

	Scope GetModule() {
		Scope scope;
		//scope.Set("Print", Value(&Print));
		//scope.Set("PrintWarning", Value(&PrintWarning));
		//scope.Set("PrintError", Value(&PrintError));
		//scope.Set("Input", Value(&Input));
		return scope;
	}

protected:

	

};

#endif
