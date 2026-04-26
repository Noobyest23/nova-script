#ifndef NOVASCRIPT_VALUE_H
#define NOVASCRIPT_VALUE_H
#include <string>
#include <unordered_map>
#include "../NovaScript_API.h"
#include <memory>

struct NOVASCRIPT_API NovaValue {
	
	enum NovaOperator {
		Plus,
		Minus,
		Multiply,
		Divide,
		Equality,
		GreaterThen,
		LesserThen,
		GreaterEqual,
		LesserEqual,
		NotEqual,
		CompoundPlus,
		CompoundMinus,
		CompoundMultiply,
		CompoundDivide,
		Mod,
		CompoundMod,
	};

	virtual std::string ToString() const = 0;
	virtual std::string Type() const = 0;
	virtual std::shared_ptr<NovaValue> Copy() const = 0;
	virtual std::shared_ptr<NovaValue> CopyPtr() { PushWarning(Type() + " cannot be passed by ptr, this warning likely came from passing a variable through dll boundries"); return nullptr; };
	virtual std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const = 0;
	virtual bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) = 0;
	virtual bool Assign(std::shared_ptr<NovaValue> rhs) = 0;
	virtual std::unordered_map<std::string, std::shared_ptr<NovaValue>> GetFullAccessableList();
	
	void OnDestroy();

	static std::string OpToString(const NovaOperator& op) {
		switch (op) {
		case NovaOperator::Plus: return "+";
		case NovaOperator::Minus: return "-";
		case NovaOperator::Multiply: return "*";
		case NovaOperator::Divide: return "/";
		case NovaOperator::Mod: return "%";
		case NovaOperator::Equality: return "==";
		case NovaOperator::GreaterThen: return ">";
		case NovaOperator::LesserThen: return "<";
		case NovaOperator::GreaterEqual: return ">=";
		case NovaOperator::LesserEqual: return "<=";
		case NovaOperator::NotEqual: return "!=";
		case NovaOperator::CompoundPlus: return "+=";
		case NovaOperator::CompoundMinus: return "-=";
		case NovaOperator::CompoundMultiply: return "*=";
		case NovaOperator::CompoundDivide: return "/=";
		case NovaOperator::CompoundMod: return "%=";
		}
	}

	std::unordered_map<std::string, std::shared_ptr<NovaValue>>* accessables = nullptr;

protected:

	virtual void on_destroy() {};
	~NovaValue() = default;

	void PushError(const std::string& msg) const;
	void PushWarning(const std::string& msg) const;
	
	static std::unordered_map<std::string, std::shared_ptr<NovaValue>> default_static_accessables;
};

#endif
