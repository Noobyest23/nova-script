#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FloatValue.h"
#include "../NovaScript/Value/BoolValue.h"

#define r(m) f##m

int* NovaInt::Num() {
	if (is_cpp) {
		return cppnum;
	}
	return &novanum;
}

const int NovaInt::CNum() const {
	if (is_cpp) {
		return *cppnum;
	}
	return novanum;
}

NovaValue* NovaInt::Copy() {
	NovaInt* i = new NovaInt(CNum());
	return i;
}

std::string NovaInt::ToString() const {
	return std::to_string(CNum());
}

std::string NovaInt::Type() const {
	return "Int";
}

NovaValue* NovaInt::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
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
		case NovaOperator::Mod:
			result %= r->CNum();
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
		default:
			OpFailed(rhs, op);
		}

		return new NovaInt(result);
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
			result = r(mod)(result, r->CNum());
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
		default:
			OpFailed(rhs, op);
		}


		return new NovaFloat(result);
	}
	
	OpFailed(rhs, op);
	return nullptr;
}

NovaValue* NovaInt::PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) {
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
			*Num() %= r->CNum();
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

NovaValue* NovaInt::Assign(NovaValue* rhs) {
	// Int can be assigned by Int or Float
	if (rhs->Type() == "Int") {
		NovaInt* i = static_cast<NovaInt*>(rhs);
		*Num() = i->CNum();
		return this;
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* f = static_cast<NovaFloat*>(rhs);
		*Num() = int(f->CNum());
		return this;
	}
	return nullptr;
}

void NovaInt::OnDestroy() {
	delete this;
}