#ifndef NOVASCRIPT_STD_MACROS
#define NOVASCRIPT_STD_MACROS

#include "../Value/Value.h"
#include "../Value/StringValue.h"

#define null_value nullptr

#define nova_std_decl(name) static NovaValue* name(std::vector<NovaValue*>& args)
#define req_args(num) if (args.size() < num) { PushError("Expected " + std::string(#num) + " args in function call"); return nullptr; }

#define strget(str, arg_i) if (args[arg_i]->Type() != "String") {\
PushError("Expected string as argument " + std::to_string(arg_i)); return nullptr;}\
NovaString* str = static_cast<NovaString*>(args[arg_i]);

#define intget(i, arg_i) if (args[arg_i]->Type() != "Int") {\
PushError("Expected string as argument " + std::to_string(arg_i)); return nullptr;}\
NovaInt* i = static_cast<NovaInt*>(args[arg_i]);

#define floatget(f, arg_i) if (args[arg_i]->Type() != "Float") {\
PushError("Expected string as argument " + std::to_string(arg_i)); return nullptr;}\
NovaFloat* f = static_cast<NovaFloat*>(args[arg_i]);

#endif