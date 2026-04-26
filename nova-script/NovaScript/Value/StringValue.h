#ifndef NOVASCRIPT_STRING_VALUE_H
#define NOVASCRIPT_STRING_VALUE_H
#include "Value.h"

struct NovaInt;
struct NovaBool;

struct NOVASCRIPT_API NovaString : public NovaValue {
	NovaString(const std::string& str);
	NovaString(std::string* cppstr);
	std::string novastr;
	std::string* cppstr = nullptr;

	std::string* Str();
	const std::string& CStr() const;

	std::string ToString() const override;
	std::string Type() const override;
	std::shared_ptr<NovaValue> Copy() const override;
	std::shared_ptr<NovaValue> CopyPtr() override;
	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const override;
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) override;
	bool Assign(std::shared_ptr<NovaValue> rhs) override;

	std::unordered_map<std::string, std::shared_ptr<NovaValue>> GetFullAccessableList() override;

	static std::unordered_map<std::string, std::shared_ptr<NovaValue>> string_accessables;

};


#endif