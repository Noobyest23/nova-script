#ifndef NOVASCRIPT_STD_MACROS
#define NOVASCRIPT_STD_MACROS

#include "../Value/Value.h"
#include "../Value/StringValue.h"

#define null_value nullptr

#define nova_std_decl(name) static NovaValue* name(std::vector<NovaValue*>& args)
#define req_args(num) if (args.size() < num) { PushError("Expected " + std::string(#num) + " args in function call"); return nullptr; }

#define strget(str, arg_i) if (args[arg_i] and args[arg_i]->Type() != "String") {\
PushError("Expected string as argument " + std::to_string(arg_i)); return nullptr;}\
NovaString* str = static_cast<NovaString*>(args[arg_i]);

#define intget(i, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Int") {\
PushError("Expected int as argument " + std::to_string(arg_i)); return nullptr;}\
NovaInt* i = static_cast<NovaInt*>(args[arg_i]);

#define numfget(f, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Int" and args[arg_i]->Type() != "Float") {\
PushError("Expected number as argument " + std::to_string(arg_i)); return nullptr;}\
float f = 0.0f;\
if (args[arg_i]->Type() != "Int") {\
NovaInt* nova_int = static_cast<NovaInt*>(args[arg_i]);\
f = nova_int->CNum();\
}\
else {\
NovaFloat* nova_float = static_cast<NovaFloat*>(args[arg_i]); \
f = nova_float->CNum();\
}

#define numiget(f, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Int" and args[arg_i]->Type() != "Float") {\
PushError("Expected number as argument " + std::to_string(arg_i)); return nullptr;}\
int f = 0;\
if (args[arg_i]->Type() != "Int") {\
NovaInt* nova_int = static_cast<NovaInt*>(args[arg_i]);\
f = nova_int->CNum();\
}\
else {\
NovaFloat* nova_float = static_cast<NovaFloat*>(args[arg_i]); \
f = nova_float->CNum();\
}

#define floatget(f, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Float") {\
PushError("Expected float as argument " + std::to_string(arg_i)); return nullptr;}\
NovaFloat* f = static_cast<NovaFloat*>(args[arg_i]);

#define boolget(b, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Boolean") {\
PushError("Expected boolean as argument " + std::to_string(arg_i)); return nullptr;}\
NovaBool* b = static_cast<NovaBool*>(args[arg_i]);

#define objget(o, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Object") {\
PushError("Expected object as argument " + std::to_string(arg_i)); return nullptr;}\
NovaObject* o = static_cast<NovaObject*>(args[arg_i]);

#define arrget(a, arg_i) if (args[arg_i] and args[arg_i]->Type() != "Array") {\
PushError("Expected array as argument " + std::to_string(arg_i)); return nullptr;}\
NovaArray* a = static_cast<NovaArray*>(args[arg_i]);

#define funcget(f, arg_i) if (args[arg_i] and args[arg_i]->Type() != "NovaFunction") {\
PushError("Expected function as argument " + std::to_string(arg_i)); return nullptr;}\
NovaFunction* f = static_cast<NovaFunction*>(args[arg_i]);

#define objbindmethod(objname, name) objname->PushBack(#name, new NovaFunction(name))

#endif