#include "../NovaScript/Value/BoolValue.h"

bool* NovaBool::B() {
	if (cppb) {
		return cppb;
	}
	return &novab;
}

const bool& NovaBool::CB() const {
	if (cppb) {
		return *cppb;
	}
	return novab;
}

NovaValue* NovaBool::Copy() {
	return new NovaBool(CB());
}

std::string NovaBool::ToString() const {
	return CB() ? "true" : "false";
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
			result = CB() == r->CB();
			break;
		case NovaOperator::NotEqual:
			result = CB() != r->CB();
			break;
		default:
			OpFailed(rhs, op);
			break;
		}
	}
	OpFailed(rhs, op);
	return nullptr;
}

NovaValue* NovaBool::Assign(NovaValue* rhs) {
	if (rhs->Type() == "Boolean") {
		NovaBool* b = static_cast<NovaBool*>(rhs);
		*B() = b->CB();
		return this;
	}
	return nullptr;
}

void NovaBool::OnDestroy() {
	delete this;
}