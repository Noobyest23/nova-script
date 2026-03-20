#ifndef NOVASCRIPT_VALUE_H
#define NOVASCRIPT_VALUE_H

#include <variant>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <functional>

#include "Scope.h"
#include "../../NovaScript_API.h"

struct Value;
class Interpretor;
struct FuncDeclNode;
struct TypeDeclNode;

using CPPFunction = Value(*)(std::vector<Value*>&);
using NovaFunction = FuncDeclNode*;
using NovaType = Scope;

using CPPVariable = std::variant<
	std::monostate,
	std::reference_wrapper<int>,
	std::reference_wrapper<float>,
	std::reference_wrapper<bool>,
	std::reference_wrapper<std::string>,
	std::reference_wrapper<glm::vec2>,
	std::reference_wrapper<glm::vec3>,
	std::reference_wrapper<glm::vec4>
>;

using ReferenceValue = std::reference_wrapper<Value>;
using NovaTypeDecl = TypeDeclNode*;

struct CPPObject {
	void* ptr = nullptr;
	Scope scope;
};

struct NOVASCRIPT_API Value {
	Value() = default;

	explicit Value(int v) : data(v) {}
	explicit Value(float v) : data(v) {}
	explicit Value(bool v) : data(v) {}
	explicit Value(std::string v) : data(std::move(v)) {}

	Value(CPPFunction v) : data(v) {}
	Value(NovaFunction v) : data(v) {}
	Value(std::vector<Value>& v) : data(v) {}
	Value(NovaType& v) : data(v) {}
	Value(NovaTypeDecl& v) : data(v) {}
	Value(CPPObject v) : data(v) {}

	Value(glm::vec2 v) : data(v) {}
	Value(glm::vec3 v) : data(v) {}
	Value(glm::vec4 v) : data(v) {}

	explicit Value(CPPVariable v) : data(v) {}
	explicit Value(ReferenceValue v) : data(v) {}

	~Value();

	std::variant<
		std::monostate, // 0
		int,
		float,
		bool,
		std::string,
		std::vector<Value>, // 5
		NovaType,
		NovaFunction,
		CPPFunction,
		CPPVariable,
		NovaTypeDecl, // 10
		CPPObject,
		glm::vec2,
		glm::vec3,
		glm::vec4,
		ReferenceValue // 15
	> data;

	std::string Type() const;
	std::string ToString() const;

	std::string& GetString();
	int& GetInt();
	float& GetFloat();
	float GetNum();
	bool& GetBool();
	glm::vec2& GetVec2();
	glm::vec3& GetVec3();
	glm::vec4& GetVec4();
	std::vector<Value>& GetArray();

	bool IsInt() const;
	bool IsFloat() const;
	bool IsNum() const;
	bool IsString() const;
	bool IsBool() const;
	bool IsFunction() const;
	bool IsVec() const;
	bool IsVec2() const;
	bool IsVec3() const;
	bool IsVec4() const;
	bool IsCPP() const;
	bool IsObj() const;
	bool IsArray() const;
	bool IsReference() const;

	bool is_manually_created = false;

	void Release() {
		if (is_manually_created) delete this;
	}
};

#endif
