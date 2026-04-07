#ifndef NOVASCRIPT_STD_OS_H
#define NOVASCRIPT_STD_OS_H
#include "NovaModule.h"
#include "nova_std_macro.h"
#include "../Value/IntValue.h"
#include "../Value/FloatValue.h"
#include "../Value/NovaObject.h"
#include "../Value/FunctionValue.h"

#ifdef _WIN32
#include "Windows.h"
class NovaOSModule : public NovaModule {
public:
	nova_std_decl(Wait) {
		req_args(1);
		numiget(wait_time, 0);
		Sleep(wait_time);
		return nullptr;
	}

	nova_std_decl(Run) {
		req_args(1);
		strget(string, 0);
		int result = system(string->CStr().c_str());
		return new NovaInt(result);
	}

	nova_std_decl(Exit) {
		if (args.size() > 0) {
			strget(string, 0);
			Callbacker::ExitCallback(string->CStr());
		}
		Callbacker::ExitCallback("");
	}

	NovaObject* GetModule() {
		NovaObject* obj = new NovaObject();
		objbindmethod(obj, Wait);
		objbindmethod(obj, Run);
		objbindmethod(obj, Exit);
		return obj;
	}

};

#endif

#endif