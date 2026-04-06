#include "../NovaScript/Value/ArrayValue.h"

std::vector<NovaValue*>* NovaArray::Arr() {
	if (cpparr) {
		return cpparr;
	}
	return &novaarr;
}

const std::vector<NovaValue*>& NovaArray::CArr() const {
	if (cpparr) {
		return *cpparr;
	}
	return novaarr;
}

NovaValue* NovaArray::Copy() {
	std::vector<NovaValue*> new_arr;
	for (NovaValue* v : CArr()) {
		new_arr.push_back(v->Copy());
	}
	return new NovaArray(new_arr);
}

std::string NovaArray::ToString() const {
	std::string output = "[";
	for (NovaValue* v : CArr()) {
		output += v ? v->ToString() : "null";
		if (v != CArr().back()) {
			output += ", ";
		}
	}
	output += "]";
	return output;
}

std::string NovaArray::Type() const {
	return "Array";
}

void NovaArray::OnDestroy() {
	for (NovaValue* v : CArr()) {
		if (v) {
			v->Release();
		}
	}
	delete this;
}

NovaValue* NovaArray::Assign(NovaValue* rhs) {
	if (rhs->Type() == "Array") {
		NovaArray* arr = static_cast<NovaArray*>(rhs);
		*Arr() = arr->CArr();
		return this;
	}
	return nullptr;
}

NovaArray::NovaArray(std::vector<NovaValue*>& arr) : novaarr(arr) {
	Init();
}

NovaArray::NovaArray(std::vector<NovaValue*>* arr) : cpparr(arr) {
	Init();
};

#include "../NovaScript/Library/nova_std_macro.h"
#include "../NovaScript/NovaErrorPush.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Value/IntValue.h"
static void PushError(std::string msg, int sevarity = 2) {
	Callbacker::PushError(msg, sevarity);
}

nova_std_decl(PushBack) {
	req_args(2);
	arrget(arr, 0);
	NovaValue* ptr = args[1];
	ptr->AddRef();
	arr->Arr()->push_back(ptr);
}

nova_std_decl(Erase) {
	req_args(2);
	arrget(arr, 0);
	intget(index, 1);

	auto* vec = arr->Arr();
	if (index->CNum() >= 0 && index->CNum() < vec->size()) {
		NovaValue* val = (*vec)[index->CNum()];
		if (val) val->Release(); // Clean up reference count
		vec->erase(vec->begin() + index->CNum());
	}
	else {
		PushError("Array index out of bounds in Erase()");
	}
	return null_value;
}

nova_std_decl(Size) {
	req_args(1);
	arrget(arr, 0);
	return new NovaInt(arr->Arr()->size()); // Assuming NovaNumber exists
}

nova_std_decl(Pop) {
	req_args(1);
	arrget(arr, 0);
	auto* vec = arr->Arr();
	if (!vec->empty()) {
		NovaValue* last = vec->back();
		vec->pop_back();
		return last;
	}
	return null_value;
}

nova_std_decl(Clear) {
	req_args(1);
	arrget(arr, 0);
	auto* vec = arr->Arr();
	for (NovaValue* v : *vec) {
		if (v) v->Release();
	}
	vec->clear();
	return null_value;
}

void NovaArray::Init() {
	accessables = new std::unordered_map<std::string, NovaValue*>{
		{"PushBack", new NovaFunction(PushBack)},
		{"Erase", new NovaFunction(Erase)},
		{"Size", new NovaFunction(Size)},
		{"Pop", new NovaFunction(Pop)},
		{"Clear", new NovaFunction(Clear)},
		{"this", nullptr}
	};
}
