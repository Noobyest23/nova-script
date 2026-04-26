#include "../NovaScript/Value/ArrayValue.h"


std::vector<std::shared_ptr<NovaValue>>* NovaArray::Arr() {
	if (cpparr) {
		return cpparr;
	}
	return &novaarr;
}

const std::vector<std::shared_ptr<NovaValue>>& NovaArray::CArr() const {
	if (cpparr) {
		return *cpparr;
	}
	return novaarr;
}

std::shared_ptr<NovaValue> NovaArray::Copy() const {
	std::vector<std::shared_ptr<NovaValue>> new_arr;
	for (std::shared_ptr<NovaValue> v : CArr()) {
		new_arr.push_back(v->Copy());
	}
	return std::make_shared<NovaArray>(new_arr);
}

std::shared_ptr<NovaValue> NovaArray::CopyPtr() {
	return std::make_shared<NovaArray>(Arr());
}

std::string NovaArray::ToString() const {
	std::string output = "[";
	for (std::shared_ptr<NovaValue> v : CArr()) {
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

bool NovaArray::Assign(std::shared_ptr<NovaValue> rhs) {
	if (rhs->Type() == "Array") {
		NovaArray* arr = static_cast<NovaArray*>(rhs.get());
		*Arr() = arr->CArr();
		return true;
	}
	return false;
}

NovaArray::NovaArray(std::vector<std::shared_ptr<NovaValue>>& arr) : novaarr(arr) {
}

NovaArray::NovaArray(const std::vector<std::shared_ptr<NovaValue>>& arr) : novaarr(arr) {
}

NovaArray::NovaArray(std::vector<std::shared_ptr<NovaValue>>* arr) : cpparr(arr) {
}

#include "../NovaScript/Library/nova_std_macro.h"
#include "../NovaScript/NovaErrorPush.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Value/IntValue.h"
static void PushError(std::string msg, int sevarity = 2) {
	Callbacker::PushError(msg, sevarity);
}

nova_std_decl(PushBack) {
	req_args(2);
	if (args[0] and args[0]->Type() != "Array") {
		PushError("Expected array as argument " + std::to_string(0)); return nullptr;
	}NovaArray* arr = static_cast<NovaArray*>(args[0].get());;
	std::shared_ptr<NovaValue> ptr = args[1];
	arr->Arr()->push_back(ptr);
	return nullptr;
}

nova_std_decl(Erase) {
	req_args(2);
	arrget(arr, 0);
	intget(index, 1);

	auto* vec = arr->Arr();
	if (index->CNum() >= 0 && index->CNum() < vec->size()) {
		std::shared_ptr<NovaValue> val = (*vec)[index->CNum()];

		vec->erase(vec->begin() + index->CNum());
	}
	else {
		PushError("Array index out of bounds in Erase()");
	}
	return nullptr;
}

nova_std_decl(Find) {
	req_args(2);
	arrget(arr, 0);
	std::shared_ptr<NovaValue> rhs = args[1];

	for (int i = 0; i < arr->CArr().size(); i++) {
		std::shared_ptr<NovaValue> lhs = arr->CArr()[i];
		if (lhs->ToString() == rhs->ToString()) {
			return std::make_shared<NovaInt>(i);
		}
	}

	return nullptr;
}

nova_std_decl(Size) {
	req_args(1);
	arrget(arr, 0);
	return std::make_shared<NovaInt>(arr->Arr()->size());
}

nova_std_decl(Pop) {
	req_args(1);
	arrget(arr, 0);
	auto* vec = arr->Arr();
	if (!vec->empty()) {
		std::shared_ptr<NovaValue> last = vec->back();
		vec->pop_back();
		return last;
	}
	return nullptr;
}

nova_std_decl(Clear) {
	req_args(1);
	arrget(arr, 0);
	auto* vec = arr->Arr();
	vec->clear();
	return nullptr;
}

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaArray::array_accessables = {
	{"PushBack", std::make_shared<NovaFunction>(PushBack, true)},
	{"Erase", std::make_shared<NovaFunction>(Erase, true)},
	{"Size", std::make_shared<NovaFunction>(Size, true)},
	{"Pop", std::make_shared<NovaFunction>(Pop, true)},
	{"Clear", std::make_shared<NovaFunction>(Clear, true)},
	{"Find", std::make_shared<NovaFunction>(Find, true)},
	{"_NOVA_THIS", nullptr }
};

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaArray::GetFullAccessableList() {
	std::unordered_map<std::string, std::shared_ptr<NovaValue>> map = NovaValue::GetFullAccessableList();

	for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : array_accessables) {
		map[pair.first] = pair.second;
	}

	return map;
}
