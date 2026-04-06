#include "../NovaScript/Value/NovaObject.h"

NovaValue* NovaObject::Copy() {
	// all objects are ptrs
	return this;
}

std::string NovaObject::ToString() const {
	std::string output = "{\n";
	for (std::pair<std::string, NovaValue*> pair : *accessables) {
		output += pair.first + " : " + pair.second->ToString() + "\n";
	}
	output += "}";
	return output;
}

std::string NovaObject::Type() const {
	return "Object";
}

void NovaObject::PushBack(const std::string& str, NovaValue* value) {
	accessables->emplace(str, value);
}

NovaValue* NovaObject::Assign(NovaValue* rhs) {
	return nullptr;
}

void NovaObject::OnDestroy() {
	delete this;
}
