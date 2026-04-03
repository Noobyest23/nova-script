#include "../NovaScript/Value/Vector2Value.h"
#include "../NovaScript/Value/FloatValue.h"
#include "../NovaScript/Value/IntValue.h"

NovaVector2::NovaVector2(float x, float y) : novax(x), novay(y) {
	accessables = new std::unordered_map<std::string, NovaValue*>;
	(*accessables)["x"] = new NovaFloat(std::reference_wrapper(X()));
	(*accessables)["y"] = new NovaFloat(std::reference_wrapper(Y()));
}

NovaVector2::NovaVector2(std::reference_wrapper<float> x, std::reference_wrapper<float> y) : cppx(x), cppy(y) {
	accessables = new std::unordered_map<std::string, NovaValue*>;
	(*accessables)["x"] = new NovaFloat(std::reference_wrapper(X()));
	(*accessables)["y"] = new NovaFloat(std::reference_wrapper(Y()));
}

float& NovaVector2::X() {
	return cppx.get();
}

float& NovaVector2::Y() {
	return cppy.get();
}

const float& NovaVector2::CX() const {
	return cppx.get();
}

const float& NovaVector2::CY() const {
	return cppy.get();
}

NovaValue* NovaVector2::Copy() const {
	return new NovaVector2(CX(), CY());
}

std::string NovaVector2::Type() const {
	return "Vector2";
}

std::string NovaVector2::ToString() const {
	return "Vector2(" + std::to_string(CX()) + ", " + std::to_string(CY()), ")";
}

NovaValue* NovaVector2::Assign(NovaValue* rhs) {
	if (rhs->Type() == "Vector2") {
		NovaVector2* vector = static_cast<NovaVector2*>(rhs);
		X() = vector->CX();
		Y() = vector->CY();
		return this;
	}
	return nullptr;
}

NovaValue* NovaVector2::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
	if (rhs->Type() == "Vector2") {
		NovaVector2* vector = static_cast<NovaVector2*>(rhs);
		switch (op) {
		case NovaOperator::Plus:
			return new NovaVector2(CX() + vector->CX(), CY() + vector->CY());
		case NovaOperator::Minus:
			return new NovaVector2(CX() - vector->CX(), CY() - vector->CY());
		case NovaOperator::Multiply:
			return new NovaVector2(CX() * vector->CX(), CY() * vector->CY());
		case NovaOperator::Divide:
			return new NovaVector2(CX() / vector->CX(), CY() / vector->CY());
		}
	}
	else if (rhs->Type() == "Int") {
		NovaInt* inum = static_cast<NovaInt*>(rhs);
		switch (op) {
		case NovaOperator::Plus:
			return new NovaVector2(CX() + inum->CNum(), CY() + inum->CNum());
		case NovaOperator::Minus:
			return new NovaVector2(CX() - inum->CNum(), CY() - inum->CNum());
		case NovaOperator::Multiply:
			return new NovaVector2(CX() * inum->CNum(), CY() * inum->CNum());
		case NovaOperator::Divide:
			return new NovaVector2(CX() / inum->CNum(), CY() / inum->CNum());
		}
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* inum = static_cast<NovaFloat*>(rhs);
		switch (op) {
		case NovaOperator::Plus:
			return new NovaVector2(CX() + inum->CNum(), CY() + inum->CNum());
		case NovaOperator::Minus:
			return new NovaVector2(CX() - inum->CNum(), CY() - inum->CNum());
		case NovaOperator::Multiply:
			return new NovaVector2(CX() * inum->CNum(), CY() * inum->CNum());
		case NovaOperator::Divide:
			return new NovaVector2(CX() / inum->CNum(), CY() / inum->CNum());
		}
	}
	OpFailed(rhs, op);
	return nullptr;
}

NovaValue* NovaVector2::PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) {
	if (rhs->Type() == "Vector2") {
		NovaVector2* vector = static_cast<NovaVector2*>(rhs);
		switch (op) {
		case NovaOperator::Plus:
			X() += vector->CX();
			Y() += vector->CY();
			return this;
		case NovaOperator::Minus:
			X() -= vector->CX();
			Y() -= vector->CY();
			return this;
		case NovaOperator::Multiply:
			X() *= vector->CX();
			Y() *= vector->CY();
			return this;
		case NovaOperator::Divide:
			X() /= vector->CX();
			Y() /= vector->CY();
			return this;
		}
	}
	else if (rhs->Type() == "Int") {
		NovaInt* num = static_cast<NovaInt*>(rhs);
		switch (op) {
		case NovaOperator::Plus:
			X() += num->CNum();
			Y() += num->CNum();
			return this;
		case NovaOperator::Minus:
			X() -= num->CNum();
			Y() -= num->CNum();
			return this;
		case NovaOperator::Multiply:
			X() *= num->CNum();
			Y() *= num->CNum();
			return this;
		case NovaOperator::Divide:
			X() /= num->CNum();
			Y() /= num->CNum();
			return this;
		}
	}
	else if (rhs->Type() == "Float") {
		NovaFloat* num = static_cast<NovaFloat*>(rhs);
		switch (op) {
		case NovaOperator::Plus:
			X() += num->CNum();
			Y() += num->CNum();
			return this;
		case NovaOperator::Minus:
			X() -= num->CNum();
			Y() -= num->CNum();
			return this;
		case NovaOperator::Multiply:
			X() *= num->CNum();
			Y() *= num->CNum();
			return this;
		case NovaOperator::Divide:
			X() /= num->CNum();
			Y() /= num->CNum();
			return this;
		}
	}
	OpFailed(rhs, op);
	return nullptr;
}