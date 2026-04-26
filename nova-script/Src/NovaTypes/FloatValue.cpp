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

std::shared_ptr<NovaValue> NovaFloat::Copy() const {
	return std::make_shared<NovaFloat>(CNum());
}

std::string NovaFloat::ToString() const {
	return std::to_string(CNum());
}

std::string NovaFloat::Type() const {
	return "Float";
}

std::shared_ptr<NovaValue> NovaFloat::PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const {
	if (rhs->Type() == "Int") {
		NovaInt* r = static_cast<NovaInt*>(rhs.get());
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
		case NovaOperator::Divide: {
			float f_result = float(result) / r->CNum();
			return std::make_shared<NovaFloat>(f_result);
			break;
		}
		case NovaOperator::Equality: {
			bool b = result == r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::NotEqual: {
			bool b = result != r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::GreaterThen: {
			bool b = result > r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::LesserThen: {
			bool b = result < r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::GreaterEqual: {
			bool b = result >= r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::LesserEqual: {
			bool b = result <= r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::Mod: {
			result %= r->CNum();
			break;
		}
		default:
			return nullptr;
		}

		return std::make_shared<NovaFloat>(result);
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* r = static_cast<NovaFloat*>(rhs.get());
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
			break;
		case NovaOperator::Equality: {
			bool b = result == r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::NotEqual: {
			bool b = result != r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::GreaterThen: {
			bool b = result > r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::LesserThen: {
			bool b = result < r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::GreaterEqual: {
			bool b = result >= r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::LesserEqual: {
			bool b = result <= r->CNum();
			return std::make_shared<NovaBool>(b);
		}
		default:
			return nullptr;
		}

		return std::make_shared<NovaFloat>(result);
	}

	return nullptr;
}


bool NovaFloat::PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) {
	if (rhs->Type() == "Int") {
		NovaInt* r = static_cast<NovaInt*>(rhs.get());
		int result = CNum();
		switch (op) {
		case NovaOperator::CompoundPlus: {
			*Num() += r->CNum();
			break;
		}
		case NovaOperator::CompoundMinus: {
			*Num() -= r->CNum();
			break;
		}
		case NovaOperator::CompoundMultiply: {
			*Num() *= r->CNum();
			break;
		}
		case NovaOperator::CompoundDivide: {
			*Num() /= r->CNum();
			break;
		}
		case NovaOperator::CompoundMod: {
			*Num() = r(mod)(CNum(), r->CNum());
			break;
		}
		default:
			return false;
		}

		return true;
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* r = static_cast<NovaFloat*>(rhs.get());
		float result = CNum();
		switch (op) {
		case NovaOperator::CompoundPlus: {
			*Num() += r->CNum();
			break;
		}
		case NovaOperator::CompoundMinus: {
			*Num() -= r->CNum();
			break;
		}
		case NovaOperator::CompoundMultiply: {
			*Num() *= r->CNum();
			break;
		}
		case NovaOperator::CompoundDivide: {
			*Num() /= r->CNum();
			break;
		}
		case NovaOperator::CompoundMod: {
			*Num() = r(mod)(CNum(), r->CNum());
			break;
		}
		default:
			return false;
			break;
		}

		return true;
	}

	return false;
}

bool NovaFloat::Assign(std::shared_ptr<NovaValue> rhs) {
	// Float can be assigned by Int or Float
	if (rhs->Type() == "Float") {
		NovaFloat* f = static_cast<NovaFloat*>(rhs.get());
		*Num() = f->CNum();
		return true;
	}
	return false;
}

