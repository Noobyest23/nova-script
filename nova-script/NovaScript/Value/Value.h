#ifndef NOVASCRIPT_VALUE_H
#define NOVASCRIPT_VALUE_H
#include <string>
#include <unordered_map>
struct NovaValue {
	
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
		NotEqual
	};

	void AddRef();
	void Release();
	virtual NovaValue* Copy() const = 0;
	virtual std::string ToString() const = 0;
	virtual std::string Type() const = 0;
	virtual NovaValue* PerformOp(NovaValue* rhs, const NovaOperator& op) const { return nullptr; };
	NovaValue* Access(const std::string& chain);
	std::unordered_map<std::string, NovaValue*>* accessables = nullptr;
	
protected:

	virtual void OnDestroy() {};
	~NovaValue() = default;

	void OpFailed(NovaValue* rhs, const NovaOperator& op) const;
	void PushError(const std::string& msg) const;
	void PushWarning(const std::string& msg) const;

	unsigned int ref_count = 1;
};

#endif
