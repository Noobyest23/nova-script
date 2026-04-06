#ifndef NOVASCRIPT_STD_TYPES_H
#define NOVASCRIPT_STD_TYPES_H
#include "nova_std_macro.h"
#include "../Value/Value.h"
#include "../Value/IntValue.h"
#include "../Value/FloatValue.h"
#include "../Value/ArrayValue.h"
#include "../Value/StringValue.h"
#include "../Value/FunctionValue.h"
#include "../Value/NovaObject.h"
#include "../Value/BoolValue.h"
#include "NovaModule.h"

class NovaTypesModule : public NovaModule {

	nova_std_decl(Int) {
		req_args(1);
		NovaValue* ptr = args[0];
		if (ptr->Type() == "Int") {
			intget(i, 0);
			return i->Copy();
		}
		else if (ptr->Type() == "Float") {
			floatget(flt, 0);
			return new NovaInt(int(flt->CNum()));
		}
		else if (ptr->Type() == "Boolean") {
			boolget(b, 0);
			return b ? new NovaInt(1) : new NovaInt(0);
		}
		else if (ptr->Type() == "String") {
			strget(str, 0);
			return new NovaInt(std::stoi(str->CStr()));
		}
		else {
			PushError("Cannot cast " + ptr->Type() + "To a int");
			return nullptr;
		}
	}

	nova_std_decl(Float) {
		req_args(1);
		NovaValue* ptr = args[0];
		if (ptr->Type() == "Int") {
			intget(i, 0);
			return new NovaFloat(float(i->CNum()));
		}
		else if (ptr->Type() == "Float") {
			floatget(flt, 0);
			return flt->Copy();
		}
		else if (ptr->Type() == "Boolean") {
			boolget(b, 0);
			return b ? new NovaFloat(1.0f) : new NovaFloat(0.0f);
		}
		else if (ptr->Type() == "String") {
			strget(str, 0);
			return new NovaFloat(std::stof(str->CStr()));
		}
		else {
			PushError("Cannot cast " + ptr->Type() + "To a float");
			return nullptr;
		}
	}

	nova_std_decl(String) {
		req_args(1);
		if (args[0]) {
			return new NovaString(args[0]->ToString());
		}
		else {
			PushError("Value in string constructor evaluated to null");
			return nullptr;
		}
	}

	nova_std_decl(Boolean) {
		req_args(1);
		NovaValue* ptr = args[0];
		if (ptr->Type() == "Int") {
			intget(i, 0);
			return new NovaBool(bool(i->CNum()));
		}
		else if (ptr->Type() == "Float") {
			floatget(flt, 0);
			return new NovaBool(bool(flt->CNum()));
		}
		else if (ptr->Type() == "Boolean") {
			boolget(b, 0);
			return b->Copy();;
		}
		else {
			PushError("Cannot cast " + ptr->Type() + "To a boolean");
			return nullptr;
		}
	}
	
	nova_std_decl(Array) {
		NovaArray* arr = new NovaArray({});
		for (NovaValue* arg : args) {
			arr->Arr()->push_back(arg->Copy());
		}
		return arr;
	}

	NovaObject* GetModule() override {
		NovaObject* obj = new NovaObject();
		objbindmethod(obj, Int);
		objbindmethod(obj, Float);
		objbindmethod(obj, String);
		objbindmethod(obj, Boolean);
		objbindmethod(obj, Array);
		return obj;
	}

};

#endif
