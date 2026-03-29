#ifndef NOVASCRIPT_VALUE_H
#define NOVASCRIPT_VALUE_H
#include <string>

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
	
	bool constant;
protected:

	virtual void OnDestroy() {};
	~NovaValue() = default;

	void OpFailed(NovaValue* rhs, const NovaOperator& op) const;
	void PushError(const std::string& msg) const;
	void PushWarning(const std::string& msg) const;
	int ref_count = 1;

};

#endif
