#ifndef NOVASCRIPT_ARRAY_VALUE_H
#define NOVASCRIPT_ARRAY_VALUE_H
#include "Value.h"
#include <vector>

struct NovaArray : public NovaValue {
	NovaArray(std::vector<NovaValue*>& arr) : arr(arr) {};
	std::vector<NovaValue*> arr;

	NovaValue* Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;
	
protected:

	void OnDestroy() override;

};

#endif
