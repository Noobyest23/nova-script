#ifndef NOVASCRIPT_OBJECT_H
#define NOVASCRIPT_OBJECT_H
#include "Value.h"
// this class LIES, it is 104 bytes
struct NOVASCRIPT_API NovaObject : public NovaValue {
	NovaObject() { accessables = new std::unordered_map<std::string, std::shared_ptr<NovaValue>>; }

	std::shared_ptr<NovaValue> Copy() const override;
	std::string ToString() const override;
	std::string Type() const override;

	std::shared_ptr<NovaValue> PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const { return nullptr; };
	bool PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) { return false; };

	void PushBack(const std::string& str, std::shared_ptr<NovaValue> value);
	bool Assign(std::shared_ptr<NovaValue>) override;

protected:

};

#endif
