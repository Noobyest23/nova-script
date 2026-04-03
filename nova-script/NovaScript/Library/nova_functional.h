#ifndef NOVASCRIPT_STD_FUNCTIONAL
#define NOVASCRIPT_STD_FUNCTIONAL
#include "NovaModule.h"
#include "nova_std_macro.h"
#include "../Value/SignalValue.h"
#include "../Value/NovaObject.h"

class NovaFunctionalModule : public NovaModule {
public:

	nova_std_decl(Signal) {
		return new NovaSignal();
	}

	NovaObject* GetModule() {
		NovaObject* obj = new NovaObject;
		obj->PushBack("Signal", new NovaFunction(Signal));
		return obj;
	}

};



#endif