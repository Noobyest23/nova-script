#include "Value.h"
#include <iostream>
#include "Scope.h"
#include "../ASTNodes/StmtNode.h"
#include "../../pch.h"

namespace {
	inline int Index(const Value& v) {
		return static_cast<int>(v.data.index());
	}

	inline int CPPIndex(const CPPVariable& v) {
		return static_cast<int>(v.index());
	}
}

std::string Value::Type() const {
	switch (Index(*this)) {
	case 0:  return "null";
	case 1:  return "int";
	case 2:  return "float";
	case 3:  return "bool";
	case 4:  return "string";
	case 5:  return "array";
	case 6:  return "ObjectType";
	case 7:  return "Function";
	case 8:  return "C++ Function";

	case 9: {
		const CPPVariable& var = std::get<CPPVariable>(data);
		switch (CPPIndex(var)) {
		case 0: return "C++ null";
		case 1: return "C++ int";
		case 2: return "C++ float";
		case 3: return "C++ bool";
		case 4: return "C++ string";
		case 5: return "C++ vec2";
		case 6: return "C++ vec3";
		case 7: return "C++ vec4";
		default: return "Unknown C++ variable";
		}
	}

	case 10: return "ObjectType Declaration";
	case 11: return "C++ Object";
	case 12: return "Vec2";
	case 13: return "Vec3";
	case 14: return "Vec4";
	case 15: return std::get<ReferenceValue>(data).get().Type();
	default: return "Unknown type in Value::Type";
	}
}

std::string Value::ToString() const {
	switch (Index(*this)) {
	case 0: return "null";
	case 1: return std::to_string(std::get<int>(data));
	case 2: return std::to_string(std::get<float>(data));
	case 3: return std::get<bool>(data) ? "true" : "false";
	case 4: return std::get<std::string>(data);

	case 5: {
		const auto& list = std::get<std::vector<Value>>(data);
		std::string out = "[";
		for (size_t i = 0; i < list.size(); ++i) {
			out += list[i].ToString();
			if (i + 1 < list.size()) out += ", ";
		}
		return out + "]";
	}

	case 6: return "ObjectType";
	case 7: return "Function";
	case 8: return "C++ Function";

	case 9: {
		const CPPVariable& var = std::get<CPPVariable>(data);
		switch (CPPIndex(var)) {
		case 0: return "null";
		case 1: return std::to_string(std::get<std::reference_wrapper<int>>(var).get());
		case 2: return std::to_string(std::get<std::reference_wrapper<float>>(var).get());
		case 3: return std::get<std::reference_wrapper<bool>>(var).get() ? "true" : "false";
		case 4: return std::get<std::reference_wrapper<std::string>>(var).get();
		case 5: {
			glm::vec2& vec = std::get<std::reference_wrapper<glm::vec2>>(var);
			return "(" + std::to_string(vec.x) + std::to_string(vec.y) + ")";
		}
		case 6: {
			glm::vec3& vec = std::get<std::reference_wrapper<glm::vec3>>(var);
			return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")";
		}
		case 7: {
			glm::vec4& vec = std::get<std::reference_wrapper<glm::vec4>>(var);
			return "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ", " + std::to_string(vec.w) + ")";
		}
		default: return "Unknown C++ variable";
		}
	}

	case 10: return "ObjectType Declaration";
	case 11: {
		CPPObject obj = std::get<CPPObject>(data);
		std::string out = "CPPObject -> \n";
		for (std::pair<std::string, Value> pair : obj.scope.variables) {
			out += pair.second.ToString() + "\n";
		}
		return out;
	}
	case 12: {
		auto v = std::get<glm::vec2>(data);
		return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
	}
	case 13: {
		auto v = std::get<glm::vec3>(data);
		return "(" + std::to_string(v.x) + ", " +
			std::to_string(v.y) + ", " +
			std::to_string(v.z) + ")";
	}
	case 14: {
		auto v = std::get<glm::vec4>(data);
		return "(" + std::to_string(v.x) + ", " +
			std::to_string(v.y) + ", " +
			std::to_string(v.z) + ", " +
			std::to_string(v.w) + ")";
	}

	case 15:
		return std::get<ReferenceValue>(data).get().ToString();

	default:
		return "Unknown type in Value::ToString";
	}
}

std::string& Value::GetString() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetString();
	if (IsCPP())      return std::get<std::reference_wrapper<std::string>>(std::get<CPPVariable>(data));
	if (IsString())   return std::get<std::string>(data);
	throw std::exception("Type is not string");
}

int& Value::GetInt() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetInt();
	if (IsCPP())      return std::get<std::reference_wrapper<int>>(std::get<CPPVariable>(data));
	if (IsInt())      return std::get<int>(data);
	throw std::exception("Type is not int");
}

float& Value::GetFloat() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetFloat();
	if (IsCPP())      return std::get<std::reference_wrapper<float>>(std::get<CPPVariable>(data));
	if (IsFloat())    return std::get<float>(data);
	throw std::exception("Type is not float");
}

float Value::GetNum() {
	if (!IsNum()) throw std::exception("Type is not num");
	return IsInt() ? static_cast<float>(GetInt()) : GetFloat();
}

bool& Value::GetBool() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetBool();
	if (IsCPP())      return std::get<std::reference_wrapper<bool>>(std::get<CPPVariable>(data));
	if (IsBool())     return std::get<bool>(data);
	throw std::exception("Type is not bool");
}

glm::vec2& Value::GetVec2() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetVec2();
	if (IsCPP())      return std::get<std::reference_wrapper<glm::vec2>>(std::get<CPPVariable>(data));
	if (IsVec2())     return std::get<glm::vec2>(data);
	throw std::exception("Type is not vec2");
}

glm::vec3& Value::GetVec3() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetVec3();
	if (IsCPP())      return std::get<std::reference_wrapper<glm::vec3>>(std::get<CPPVariable>(data));
	if (IsVec3())     return std::get<glm::vec3>(data);
	throw std::exception("Type is not vec3");
}

glm::vec4& Value::GetVec4() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetVec4();
	if (IsCPP())      return std::get<std::reference_wrapper<glm::vec4>>(std::get<CPPVariable>(data));
	if (IsVec4())     return std::get<glm::vec4>(data);
	throw std::exception("Type is not vec4");
}

std::vector<Value>& Value::GetArray() {
	if (IsReference()) return std::get<ReferenceValue>(data).get().GetArray();
	if (IsArray())     return std::get<std::vector<Value>>(data);
	throw std::exception("Type is not array");
}

bool Value::IsReference() const { return Index(*this) == 15; }

bool Value::IsCPP() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsCPP();
	int i = Index(*this);
	return i == 8 || i == 9 || i == 11;
}

bool Value::IsInt() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsInt();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 1;
	return Index(*this) == 1;
}

bool Value::IsFloat() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsFloat();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 2;
	return Index(*this) == 2;
}

bool Value::IsNum() const { return IsInt() || IsFloat(); }

bool Value::IsString() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsString();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 4;
	return Index(*this) == 4;
}

bool Value::IsBool() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsBool();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 3;
	return Index(*this) == 3;
}

bool Value::IsFunction() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsFunction();
	int i = Index(*this);
	return i == 7 || i == 8;
}

bool Value::IsVec() const { return IsVec2() || IsVec3() || IsVec4(); }

bool Value::IsVec2() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsVec2();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 5;
	return Index(*this) == 12;
}

bool Value::IsVec3() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsVec3();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 6;
	return Index(*this) == 13;
}

bool Value::IsVec4() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsVec4();
	if (IsObj()) return false;
	if (IsCPP()) return CPPIndex(std::get<CPPVariable>(data)) == 7;
	return Index(*this) == 14;
}

bool Value::IsObj() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsObj();
	int i = Index(*this);
	return i == 6 || i == 11;
}

bool Value::IsArray() const {
	if (IsReference()) return std::get<ReferenceValue>(data).get().IsArray();
	return Index(*this) == 5;
}

Value::~Value() = default;
