#include "../NovaScript/Value/NovaObject.h"

NovaValue* NovaObject::Copy() const {
	NovaObject* obj = new NovaObject();
	std::unordered_map<std::string, NovaValue*>* accs = new std::unordered_map<std::string, NovaValue*>(*accessables);
	obj->accessables = accs;
	return obj;
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
