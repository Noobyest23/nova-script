#ifndef NOVASCRIPT_SCOPE_H
#define NOVASCRIPT_SCOPE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "../../NovaScript_API.h"
struct Value;

struct NOVASCRIPT_API Scope {
	Scope* parent = nullptr;

	std::unordered_map<std::string, Value> variables;

	Scope() = default;
	explicit Scope(std::unordered_map<std::string, Value>& variables) : variables(variables) {};
	explicit Scope(Scope* parentScope) : parent(parentScope) {}
	~Scope();

	Value* Get(const std::string& name);

	// Insert / overwrite
	void Set(const std::string& name, Value val);

	bool Has(const std::string& name);

	std::unordered_map<std::string, Value>& GetAll() { return variables; };

	std::string Print() const;

};

#endif