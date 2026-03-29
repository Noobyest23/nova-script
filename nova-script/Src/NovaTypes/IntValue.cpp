#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FloatValue.h"
#include "../NovaScript/Value/BoolValue.h"

NovaValue* NovaInt::Copy() const {
	NovaInt* i = new NovaInt(num);
	return i;
}

std::string NovaInt::ToString() const {
	return std::to_string(num);
}

std::string NovaInt::Type() const {
	return "Int";
}

NovaValue* NovaInt::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
	if (rhs->Type() == "Int") {
		NovaInt* r = static_cast<NovaInt*>(rhs);
		int result = num;
		switch (op) {
		case NovaOperator::Plus:
			result += r->num;
			break;
		case NovaOperator::Minus:
			result -= r->num;
			break;
		case NovaOperator::Multiply:
			result *= r->num;
			break;
		case NovaOperator::Divide:
			result /= r->num;
			break;
		case NovaOperator::Equality: {
			bool b = result == r->num;
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = result != r->num;
			return new NovaBool(b);
		}
		case NovaOperator::GreaterThen: {
			bool b = result > r->num;
			return new NovaBool(b);
		}
		case NovaOperator::LesserThen: {
			bool b = result < r->num;
			return new NovaBool(b);
		}
		case NovaOperator::GreaterEqual: {
			bool b = result >= r->num;
			return new NovaBool(b);
		}
		case NovaOperator::LesserEqual: {
			bool b = result <= r->num;
			return new NovaBool(b);
		}
		default:
			OpFailed(rhs, op);
		}

		return new NovaInt(result);
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* r = static_cast<NovaFloat*>(rhs);
		float result = num;
		switch (op) {
		case NovaOperator::Plus:
			result += r->num;
			break;
		case NovaOperator::Minus:
			result -= r->num;
			break;
		case NovaOperator::Multiply:
			result *= r->num;
			break;
		case NovaOperator::Divide:
			result /= r->num;
			break;
		case NovaOperator::Equality: {
			bool b = result == r->num;
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = result != r->num;
			return new NovaBool(b);
		}
		case NovaOperator::GreaterThen: {
			bool b = result > r->num;
			return new NovaBool(b);
		}
		case NovaOperator::LesserThen: {
			bool b = result < r->num;
			return new NovaBool(b);
		}
		case NovaOperator::GreaterEqual: {
			bool b = result >= r->num;
			return new NovaBool(b);
		}
		case NovaOperator::LesserEqual: {
			bool b = result <= r->num;
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