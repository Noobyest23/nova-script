#include "../NovaScript/Value/BoolValue.h"

NovaValue* NovaBool::Copy() const {
	return new NovaBool(b);
}

std::string NovaBool::ToString() const {
	return b ? "true" : "false";
}

std::string NovaBool::Type() const {
	return "Boolean";
}

NovaValue* NovaBool::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
	if (rhs->Type() == "Boolean") {
		NovaBool* r = static_cast<NovaBool*>(rhs);
		bool result;
		switch (op) {
		case NovaOperator::Equality:
			result = b == r->b;
			break;
		case NovaOperator::NotEqual:
			result = b != r->b;
			break;
		default:
			OpFailed(rhs, op);
			break;
		}
	}
	OpFailed(rhs, op);
	return nullptr;
}