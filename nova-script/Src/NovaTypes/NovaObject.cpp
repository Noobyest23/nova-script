#include "../NovaScript/Value/NovaObject.h"

std::shared_ptr<NovaValue> NovaObject::Copy() const {
	std::shared_ptr<NovaObject> result = std::make_shared<NovaObject>();
	for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : *accessables) {
		result->PushBack(pair.first, pair.second->Copy());
	}
	return result;
}

std::string NovaObject::ToString() const {
	std::string output = "{\n";
	for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : *accessables) {
		output += pair.first + " : " + pair.second->ToString() + "\n";
	}
	output += "}";
	return output;
}

std::string NovaObject::Type() const {
	return "Object";
}

void NovaObject::PushBack(const std::string& str, std::shared_ptr<NovaValue> value) {
	accessables->emplace(str, value);
}

bool NovaObject::Assign(std::shared_ptr<NovaValue> rhs) {
	return false;
}