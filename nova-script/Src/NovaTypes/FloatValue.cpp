#include "../NovaScript/Value/FloatValue.h"
#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/BoolValue.h"

#define r(m) f##m

float* NovaFloat::Num() {
	if (is_cpp) {
		cppnum;
	}
	else {
		return &novanum;
	}
}

const float NovaFloat::CNum() const {
	if (is_cpp) {
		return *cppnum;
	}
	return novanum;
}

NovaValue* NovaFloat::Copy() {
	NovaFloat* f = new NovaFloat(CNum());
	return f;
}

std::string NovaFloat::ToString() const {
	return std::to_string(CNum());
}

std::string NovaFloat::Type() const {
	return "Float";
}

NovaValue* NovaFloat::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
	if (rhs->Type() == "Int") {
		NovaInt* r = static_cast<NovaInt*>(rhs);
		int result = CNum();
		switch (op) {
		case NovaOperator::Plus:
			result += r->CNum();
			break;
		case NovaOperator::Minus:
			result -= r->CNum();
			break;
		case NovaOperator::Multiply:
			result *= r->CNum();
			break;
		case NovaOperator::Divide:
			result /= r->CNum();
			break;
		case NovaOperator::Equality: {
			bool b = result == r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = result != r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::GreaterThen: {
			bool b = result > r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::LesserThen: {
			bool b = result < r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::GreaterEqual: {
			bool b = result >= r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::LesserEqual: {
			bool b = result <= r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::Mod: {
			result %= r->CNum();
		}
		default:
			OpFailed(rhs, op);
		}

		return new NovaFloat(result);
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* r = static_cast<NovaFloat*>(rhs);
		float result = CNum();
		switch (op) {
		case NovaOperator::Plus:
			result += r->CNum();
			break;
		case NovaOperator::Minus:
			result -= r->CNum();
			break;
		case NovaOperator::Multiply:
			result *= r->CNum();
			break;
		case NovaOperator::Divide:
			result /= r->CNum();
			break;
		case NovaOperator::Mod:
			result = r(mod)(CNum(), r->CNum());
		case NovaOperator::Equality: {
			bool b = result == r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = result != r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::GreaterThen: {
			bool b = result > r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::LesserThen: {
			bool b = result < r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::GreaterEqual: {
			bool b = result >= r->CNum();
			return new NovaBool(b);
		}
		case NovaOperator::LesserEqual: {
			bool b = result <= r->CNum();
			return new NovaBool(b);
		}
		default:
			OpFailed(rhs, op);
		}

		return new NovaFloat(result);
	}
	OpFailed(rhs, op);
	return nullptr;
}


NovaValue* NovaFloat::PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) {
	if (rhs->Type() == "Int") {
		NovaInt* r = static_cast<NovaInt*>(rhs);
		int result = CNum();
		switch (op) {
		case NovaOperator::CompoundPlus: {
			*Num() += r->CNum();
			return this;
		}
		case NovaOperator::CompoundMinus: {
			*Num() -= r->CNum();
			return this;
		}
		case NovaOperator::CompoundMultiply: {
			*Num() *= r->CNum();
			return this;
		}
		case NovaOperator::CompoundDivide: {
			*Num() /= r->CNum();
			return this;
		}
		case NovaOperator::CompoundMod: {
			*Num() = r(mod)(CNum(), r->CNum());
			return this;
		}
		default:
			OpFailed(rhs, op);
		}

		return new NovaInt(result);
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* r = static_cast<NovaFloat*>(rhs);
		float result = CNum();
		switch (op) {
		case NovaOperator::CompoundPlus: {
			*Num() += r->CNum();
			return this;
		}
		case NovaOperator::CompoundMinus: {
			*Num() -= r->CNum();
			return this;
		}
		case NovaOperator::CompoundMultiply: {
			*Num() *= r->CNum();
			return this;
		}
		case NovaOperator::CompoundDivide: {
			*Num() /= r->CNum();
			return this;
		}
		case NovaOperator::CompoundMod: {
			*Num() = r(mod)(CNum(), r->CNum());
			return this;
		}
		default:
			OpFailed(rhs, op);
		}

		return new NovaFloat(result);
	}

	OpFailed(rhs, op);
	return nullptr;
}

NovaValue* NovaFloat::Assign(NovaValue* rhs) {
	// Float can be assigned by Int or Float
	if (rhs->Type() == "Int") {
		NovaInt* i = static_cast<NovaInt*>(rhs);
		*Num() = float(i->CNum());
		return this;
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* f = static_cast<NovaFloat*>(rhs);
		*Num() = f->CNum();
		return this;
	}
	return nullptr;
}

void NovaFloat::OnDestroy() {
	delete this;
}
