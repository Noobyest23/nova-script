#include "../NovaScript/Value/ArrayValue.h"

NovaValue* NovaArray::Copy() const {
	std::vector<NovaValue*> new_arr;
	for (NovaValue* v : arr) {
		new_arr.push_back(v->Copy());
	}
	return new NovaArray(new_arr);
}

std::string NovaArray::ToString() const {
	std::string output = "[";
	for (NovaValue* v : arr) {
		output += v ? v->ToString() : "null";
		if (v != arr.back()) {
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
	for (NovaValue* v : arr) {
		if (v) {
			v->Release();
		}
	}
}