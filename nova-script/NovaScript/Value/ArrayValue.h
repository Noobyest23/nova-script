#ifndef NOVASCRIPT_ARRAY_VALUE_H
#define NOVASCRIPT_ARRAY_VALUE_H
#include "Value.h"
#include <vector>

struct NOVASCRIPT_API NovaArray : public NovaValue {
	NovaArray(std::vector<std::shared_ptr<NovaValue>>& arr);
	NovaArray(const std::vector<std::shared_ptr<NovaValue>>& arr);
	NovaArray(std::vector<std::shared_ptr<NovaValue>>* arr);

	std::vector<std::shared_ptr<NovaValue>> novaarr;
	std::vector<std::shared_ptr<NovaValue>>* cpparr = nullptr;

	std::vector<std::shared_ptr<NovaValue>>* Arr();
	const std::vector<std::shared_ptr<NovaValue>>& CArr() const;

	std::shared_ptr<NovaValue> Copy() const override;
	std::shared_ptr<NovaValue> CopyPtr() override;
	std::string ToString() const override;
	std::string Type() const override;
	bool Assign(std::shared_ptr<NovaValue> rhs) override;

	std::unordered_map<std::string, std::shared_ptr<NovaValue>> GetFullAccessableList() override;

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const { return nullptr; };
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) { return false; };

	static std::unordered_map<std::string, std::shared_ptr<NovaValue>> array_accessables;

};

#endif
