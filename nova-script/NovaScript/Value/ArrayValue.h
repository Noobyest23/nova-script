#ifndef NOVASCRIPT_ARRAY_VALUE_H
#define NOVASCRIPT_ARRAY_VALUE_H
#include "Value.h"
#include <vector>

struct NOVASCRIPT_API NovaArray : public NovaValue {
	NovaArray(std::vector<NovaValue*>& arr);
	NovaArray(std::vector<NovaValue*>* arr);

	std::vector<NovaValue*> novaarr;
	std::vector<NovaValue*>* cpparr = nullptr;

	std::vector<NovaValue*>* Arr();
	const std::vector<NovaValue*>& CArr() const;

	NovaValue* Copy() override;
	std::string ToString() const override;
	std::string Type() const override;
	NovaValue* Assign(NovaValue* rhs) override;


	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const { return nullptr; };
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) { return nullptr; };

protected:

	void OnDestroy() override;

private:

	void Init();

};

#endif
