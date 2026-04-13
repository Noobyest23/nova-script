#ifndef NOVASCRIPT_STANDARD_GLOBAL_H
#define NOVASCRIPT_STANDARD_GLOBAL_H
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

class NovaGlobalModule : public NovaModule {
public:

	nova_std_decl(Range) {
		req_args(1);
		int from = 0;
		int by = 1;
		int to;

		// Argument Handling
		if (args.size() == 1) {
			numiget(t, 0);
			to = t;
		}
		else {
			numiget(f, 0);
			numiget(t, 1);
			from = f;
			to = t;
			if (args.size() > 2) {
				numiget(b, 2);
				by = b;
			}
		}

		if (by == 0) {
			PushError("Step ('by') cannot be 0");
			return nullptr;
		}

		std::vector<NovaValue*> array;

		if (by > 0) {
			
			if (from > to) {
				PushError("'from' must be less than 'to' for positive steps");
				return nullptr;
			}
			for (int i = from; i < to; i += by) {
				array.push_back(new NovaInt(i));
			}
		}
		else {
			if (from < to) {
				PushError("'from' must be greater than 'to' for negative steps");
				return nullptr;
			}
			for (int i = from; i > to; i += by) {
				array.push_back(new NovaInt(i));
			}
		}

		return new NovaArray(array);
	}

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
		else if (ptr->Type() == "Bool") {
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
		else if (ptr->Type() == "bool") {
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

	nova_std_decl(Bool) {
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
		else if (ptr->Type() == "bool") {
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
#include "../NovaScript.h"
	nova_std_decl(Execute) {
		req_args(1);
		strget(path, 0);
		InterpretorHandle i_handle = BuildScript(path->CStr().c_str());
		ExecuteScript(i_handle);
		DestroyScript(i_handle);
		return nullptr;
	}

	NovaObject* GetModule() override {
		NovaObject* obj = new NovaObject();
		objbindmethod(obj, Range);
		objbindmethod(obj, Int);
		objbindmethod(obj, Float);
		objbindmethod(obj, String);
		objbindmethod(obj, Bool);
		objbindmethod(obj, Array);
		objbindmethod(obj, Execute);
		return obj;
	}

};


#endif