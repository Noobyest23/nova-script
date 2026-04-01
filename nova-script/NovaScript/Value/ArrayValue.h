#ifndef NOVASCRIPT_ARRAY_VALUE_H
#define NOVASCRIPT_ARRAY_VALUE_H
#include "Value.h"
#include <vector>

struct NOVASCRIPT_API NovaArray : public NovaValue {
	NovaArray(std::vector<NovaValue*>& arr) : novaarr(arr) {};
	NovaArray(std::reference_wrapper<std::vector<NovaValue*>> cpparr) : cpparr(cpparr) {};
	std::vector<NovaValue*> novaarr;
	std::reference_wrapper<std::vector<NovaValue*>> cpparr = novaarr;

	std::vector<NovaValue*>& Arr();
	const std::vector<NovaValue*>& CArr() const;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;
	NovaValue* Assign(NovaValue* rhs) override;

protected:

	void OnDestroy() override;

};

#endif
