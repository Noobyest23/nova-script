#ifndef NOVASCRIPT_SCOPE_H
#define NOVASCRIPT_SCOPE_H

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "../NovaScript_API.h"
#include "../NovaScript/Value/Value.h"

struct NOVASCRIPT_API Scope {
	Scope* parent = nullptr;

	std::unordered_map<std::string, std::shared_ptr<NovaValue>> variables;

	Scope() = default;
	explicit Scope(std::unordered_map<std::string, std::shared_ptr<NovaValue>>& variables) : variables(variables) {};
	explicit Scope(Scope* parentScope) : parent(parentScope) {}
	~Scope();

	std::shared_ptr<NovaValue> Get(const std::string& name);

	// Insert / overwrite
	void Set(const std::string& name, std::shared_ptr<NovaValue> val);

	bool Has(const std::string& name);

	bool LimitedHas(const std::string& name);
	void LimitedSet(const std::string& name, std::shared_ptr<NovaValue> val);
	std::shared_ptr<NovaValue> LimitedGet(const std::string& name);

	std::unordered_map<std::string, std::shared_ptr<NovaValue>>& GetAll() { return variables; };

	std::string Print() const;

};

#endif