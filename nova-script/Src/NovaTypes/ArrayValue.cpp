#include "../NovaScript/Value/ArrayValue.h"

std::vector<NovaValue*>& NovaArray::Arr() {
	return cpparr.get();
}

const std::vector<NovaValue*>& NovaArray::CArr() const {
	return cpparr.get();
}

NovaValue* NovaArray::Copy() const {
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
}

NovaValue* NovaArray::Assign(NovaValue* rhs) {
	if (rhs->Type() == "Array") {
		NovaArray* arr = static_cast<NovaArray*>(rhs);
		this->Arr() = arr->CArr();
		return this;
	}
	return nullptr;
}
