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

std::shared_ptr<NovaValue> NovaInt::Copy() const {
	return std::make_shared<NovaInt>(CNum());
}

std::shared_ptr<NovaValue> NovaInt::CopyPtr() {
	return std::make_shared<NovaInt>(Num());
}

std::string NovaInt::ToString() const {
	return std::to_string(CNum());
}

std::string NovaInt::Type() const {
	return "Int";
}

std::shared_ptr<NovaValue> NovaInt::PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const {
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
		case NovaOperator::Mod:
			result %= r->CNum();
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

		return std::make_shared<NovaInt>(result);
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
		case NovaOperator::Divide: {
			float f_result = float(result) / r->CNum();
			return std::make_shared<NovaFloat>(f_result);
			break;
		}
		case NovaOperator::Mod:
			result = r(mod)(result, r->CNum());
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

bool NovaInt::PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) {
	if (rhs->Type() == "Int") {
		NovaInt* r = static_cast<NovaInt*>(rhs.get());
		int result = CNum();
		switch (op) {
		case NovaOperator::CompoundPlus: {
			*Num() += r->CNum();
			return true;
		}
		case NovaOperator::CompoundMinus: {
			*Num() -= r->CNum();
			return true;
		}
		case NovaOperator::CompoundMultiply: {
			*Num() *= r->CNum();
			return true;
		}
		case NovaOperator::CompoundDivide: {
			*Num() /= r->CNum();
			return true;
		}
		case NovaOperator::CompoundMod: {
			*Num() %= r->CNum();
			return true;
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
			return false;
		}

		return true;
	}

	return false;
}

bool NovaInt::Assign(std::shared_ptr<NovaValue> rhs) {
	// Int can be assigned by Int or Float
	if (rhs->Type() == "Int") {
		NovaInt* i = static_cast<NovaInt*>(rhs.get());
		*Num() = i->CNum();
		return true;
	}
	return false;
}