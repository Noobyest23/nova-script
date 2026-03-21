#ifndef NOVASCRIPT_VECTOR_LIB_H
#define NOVASCRIPT_VECTOR_LIB_H

#include "../Interpretor/Scope.h"
#include "nova_std_macro.h"
#include "NovaModule.h"

class NovaVectorModule : public NovaModule {
public:

	NovaVectorModule() {
		module_name = "vector";
	}

	nova_std_decl(Distance) {
		req_args(2);
		if (args[0]->IsVec2()) {
			vec2get(lhs, 0);
			vec2get(rhs, 0);

			return glm::vec2(glm::distance(lhs, rhs));
		}
		else if (args[0]->IsVec3()) {
			vec3get(lhs, 0);
			vec3get(rhs, 0);

			return glm::vec3(glm::distance(lhs, rhs));
		}
		else if (args[0]->IsVec4()) {
			vec4get(lhs, 0);
			vec4get(rhs, 0);

			return glm::vec4(glm::distance(lhs, rhs));
		}
		else {
			PushError("Argument 0 is not a vector!");
			return Value();
		}
	}

	nova_std_decl(Length) {
		req_args(1);
		if (args[0]->IsVec2()) {
			return Value(args[0]->GetVec2().length());
		}
		else if (args[0]->IsVec3()) {
			return Value(args[0]->GetVec3().length());
		}
		else if (args[0]->IsVec4()) {
			return Value(args[0]->GetVec4().length());
		}
		else {
			PushError("Argument 0 is not a vector!");
			return Value();
		}
	}

	nova_std_decl(Normalize) {
		req_args(1);
		if (args[0]->IsVec2()) {
			vec2get(lhs, 0);

			return glm::vec2(glm::normalize(lhs));
		}
		else if (args[0]->IsVec3()) {
			vec3get(lhs, 0);

			return glm::vec3(glm::normalize(lhs));
		}
		else if (args[0]->IsVec4()) {
			vec4get(lhs, 0);

			return glm::vec4(glm::normalize(lhs));
		}
		else {
			PushError("Argument 0 is not a vector!");
			return Value();
		}
	}

	Scope GetModule() override {
		Scope scope;
		NOVA_BIND_METHOD(Distance);
		NOVA_BIND_METHOD(Length);
		NOVA_BIND_METHOD(Normalize);
		return scope;
	};

};



#endif