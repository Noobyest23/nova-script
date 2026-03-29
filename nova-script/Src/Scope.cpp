#include "../NovaScript/Interpretor/Scope.h"
#include "../NovaScript/Value/Value.h"

Scope::~Scope() {
	for (std::pair<std::string, NovaValue*> pair : variables) {
		if (pair.second) {
			pair.second->Release();
		}
	}
	variables.clear();
}

NovaValue* Scope::Get(const std::string& name) {
	auto it = variables.find(name);
	if (it != variables.end()) {
		return it->second;
	}
	if (parent) return parent->Get(name);
	return nullptr;
}

void Scope::Set(const std::string& name, NovaValue* val) {
	auto it = variables.find(name);
	if (it != variables.end()) {
		val->AddRef();
		variables[name]->Release();
		variables[name] = val;
	}
	else if (parent) {
		if (parent->Has(name)) {
			parent->Set(name, val);
		}
		else {
			val->AddRef();
			variables.insert_or_assign(name, val);
		}
	}
	else {
		if (val) {
			val->AddRef();
		}
		variables.insert_or_assign(name, val);
	}
}

bool Scope::Has(const std::string& name) {
	auto it = variables.find(name);
	return it != variables.end();
}

std::string Scope::Print() const {
	std::string out;
	for (std::pair<std::string, NovaValue*> pair : variables) {
		out += pair.first + " = " + pair.second->ToString() + "\n";
	}
	if (parent) {
		out += "-- Parent Scope --";
		out += parent->Print();
	}
	return out;
}
