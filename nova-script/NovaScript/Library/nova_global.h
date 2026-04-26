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

		std::vector<std::shared_ptr<NovaValue>> array;

		if (by > 0) {
			
			if (from > to) {
				PushError("'from' must be less than 'to' for positive steps");
				return nullptr;
			}
			for (int i = from; i < to; i += by) {
				array.push_back(std::make_shared<NovaInt>(i));
			}
		}
		else {
			if (from < to) {
				PushError("'from' must be greater than 'to' for negative steps");
				return nullptr;
			}
			for (int i = from; i > to; i += by) {
				array.push_back(std::make_shared<NovaInt>(i));
			}
		}

		return std::make_shared<NovaArray>(array);
	}

	nova_std_decl(Int) {
		req_args(1);
		std::shared_ptr<NovaValue> ptr = args[0];
		if (ptr->Type() == "Int") {
			intget(i, 0);
			return i->Copy();
		}
		else if (ptr->Type() == "Float") {
			floatget(flt, 0);
			return std::make_shared<NovaInt>(int(flt->CNum()));
		}
		else if (ptr->Type() == "Bool") {
			boolget(b, 0);
			return b ? std::make_shared<NovaInt>(1) : std::make_shared<NovaInt>(0);
		}
		else if (ptr->Type() == "String") {
			strget(str, 0);
			try {
				return std::make_shared<NovaInt>(std::stoi(str->CStr()));
			}
			catch (std::exception e) {
				std::cout << "Invalid String to Int conversion " + str->ToString();
			}
		}
		else {
			PushError("Cannot cast " + ptr->Type() + "To a int");
			return nullptr;
		}
	}

	nova_std_decl(Float) {
		req_args(1);
		std::shared_ptr<NovaValue> ptr = args[0];
		if (ptr->Type() == "Int") {
			intget(i, 0);
			return std::make_shared<NovaFloat>(float(i->CNum()));
		}
		else if (ptr->Type() == "Float") {
			floatget(flt, 0);
			return flt->Copy();
		}
		else if (ptr->Type() == "bool") {
			boolget(b, 0);
			return b ? std::make_shared<NovaFloat>(1.0f) : std::make_shared<NovaFloat>(0.0f);
		}
		else if (ptr->Type() == "String") {
			strget(str, 0);
			try {
				return std::make_shared<NovaFloat>(std::stof(str->CStr()));
			}
			catch (std::exception e) {
				PushError("Invalid String to Float conversion (" + str->ToString() + ")");
			}
		}
		else {
			PushError("Cannot cast " + ptr->Type() + "To a float");
			return nullptr;
		}
	}

	nova_std_decl(String) {
		req_args(1);
		if (args[0]) {
			return std::make_shared<NovaString>(args[0]->ToString());
		}
		else {
			PushError("Value in string constructor evaluated to null");
			return nullptr;
		}
	}

	nova_std_decl(Bool) {
		req_args(1);
		std::shared_ptr<NovaValue> ptr = args[0];
		if (ptr->Type() == "Int") {
			intget(i, 0);
			return std::make_shared<NovaBool>(bool(i->CNum()));
		}
		else if (ptr->Type() == "Float") {
			floatget(flt, 0);
			return std::make_shared<NovaBool>(bool(flt->CNum()));
		}
		else if (ptr->Type() == "bool") {
			boolget(b, 0);
			return b->Copy();
		}
		else {
			PushError("Cannot cast " + ptr->Type() + "To a boolean");
			return nullptr;
		}
	}

	nova_std_decl(Array) {
		std::shared_ptr<NovaArray> arr = std::make_shared<NovaArray>(std::vector<std::shared_ptr<NovaValue>>{});
		for (std::shared_ptr<NovaValue> arg : args) {
			arr->Arr()->push_back(arg);
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

	std::shared_ptr<NovaObject> GetModule() override {
		std::shared_ptr<NovaObject> obj = std::make_shared<NovaObject>();
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