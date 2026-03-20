#ifndef NOVA_SCRIPT_MODULE_H
#define NOVA_SCRIPT_MODULE_H

#include <string>
#include "../../NovaErrorPush.h"
#include "nova_std_macro.h"
#include "../../NovaScript_API.h"
struct Scope;

class NOVASCRIPT_API NovaModule {
public:

	std::string module_name = "";

	virtual Scope GetModule() = 0;

	virtual ~NovaModule() = default;

protected:

	static void PushError(const std::string& message);

};

#endif