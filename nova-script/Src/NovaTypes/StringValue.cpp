#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/BoolValue.h"

NovaValue* NovaString::Copy() const {
	return new NovaString(str);
}

std::string NovaString::ToString() const {
	return str;
}

std::string NovaString::Type() const {
	return "String";
}

NovaValue* NovaString::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
	if (Type() == "String") {
		NovaString* r = static_cast<NovaString*>(rhs);
		std::string result;
		switch(op) {
		case NovaOperator::Plus:
			result = str + r->str;
			break;
		case NovaOperator::Equality: {
			bool b = str == r->str;
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = str != r->str;
			return new NovaBool(b);
		}
		default:
			OpFailed(rhs, op);
			return nullptr;
			break;
		}

		return new NovaString(result);
	}

	OpFailed(rhs, op);
	return nullptr;
}