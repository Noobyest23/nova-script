#include "Scope.h"
#include "Value.h"
#include "../../pch.h"

Scope::~Scope() {
	variables.clear();
}

Value* Scope::Get(const std::string& name) {
	auto it = variables.find(name);
	if (it != variables.end()) return &it->second;
	if (parent) return parent->Get(name);
	return nullptr;
}

void Scope::Set(const std::string& name, Value val) {
	auto it = variables.find(name);
	if (it != variables.end()) {
		variables[name] = val;
	}
	else if (parent) {
		if (parent->Has(name)) {
			parent->Set(name, val);
		}
		else {
			variables.insert_or_assign(name, val);
		}
	}
	else {
		variables.insert_or_assign(name, val);
	}
}

bool Scope::Has(const std::string& name) {
	auto it = variables.find(name);
	return it != variables.end();
}

std::string Scope::Print() const {
	std::string out;
	for (std::pair<std::string, Value> pair : variables) {
		out += pair.first + " = " + pair.second.ToString() + "\n";
	}
	if (parent) {
		out += "-- Parent Scope --";
		out += parent->Print();
	}
	return out;
}
