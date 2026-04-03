#ifndef NOVASCRIPT_VECTOR2_VALUE_H
#define NOVASCRIPT_VECTOR2_VALUE_H
#include "Value.h"

struct NovaVector2 : public NovaValue {
public:
	NovaVector2(float x, float y);
	NovaVector2(std::reference_wrapper<float> x, std::reference_wrapper<float> y);

	float novax, novay;
	std::reference_wrapper<float> cppx = novax;
	std::reference_wrapper<float> cppy = novay;

	float& X();
	float& Y();

	const float& CX() const;
	const float& CY() const;

	NovaValue* Copy() const override;
	std::string Type() const override;
	std::string ToString() const override;
	NovaValue* Assign(NovaValue* rhs) override;
	NovaValue* PerformOp(NovaValue* rhs, const NovaOperator&) const override;
	NovaValue* PerformCompoundOp(NovaValue* rhs, const NovaOperator&) override;

};



#endif