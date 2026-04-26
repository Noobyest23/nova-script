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

std::shared_ptr<NovaValue> NovaBool::Copy() const {
	return std::make_shared<NovaBool>(CB());
}

std::string NovaBool::ToString() const {
	return CB() ? "true" : "false";
}

std::string NovaBool::Type() const {
	return "Bool";
}

std::shared_ptr<NovaValue> NovaBool::PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const {
	if (rhs->Type() == "Bool") {
		NovaBool* r = static_cast<NovaBool*>(rhs.get());
		bool result;
		switch (op) {
		case NovaOperator::Equality:
			result = CB() == r->CB();
			break;
		case NovaOperator::NotEqual:
			result = CB() != r->CB();
			break;
		default:
			return nullptr;
			break;
		}

		return std::make_shared<NovaBool>(result);
	}
	return nullptr;
}

bool NovaBool::Assign(std::shared_ptr<NovaValue> rhs) {
	if (rhs->Type() == "Bool") {
		NovaBool* b = static_cast<NovaBool*>(rhs.get());
		*B() = b->CB();
		return true;
	}
	return false;
}
