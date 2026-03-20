#ifndef NOVA_SCRIPT_MATH_H
#define NOVA_SCRIPT_MATH_H
#include "NovaModule.h"
#include <algorithm>
#undef min
#undef max

class NovaMathModule : public NovaModule {

	nova_std_decl(Sin) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(sin(val));
	}

	nova_std_decl(Cos) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(cos(val));
	}

	nova_std_decl(Tan) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(tan(val));
	}

	nova_std_decl(Sqrt) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(sqrt(val));
	}

	nova_std_decl(Abs) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(abs(val));
	}

	nova_std_decl(Round) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(round(val));
	}

	nova_std_decl(Floor) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(floor(val));
	}

	nova_std_decl(Ceil) {
		req_args(1);
		float val = std::get<float>(args[0]->data);
		return Value(ceil(val));
	}

	nova_std_decl(Min) {
		req_args(2);
		float val1 = std::get<float>(args[0]->data);
		float val2 = std::get<float>(args[1]->data);
		return Value(std::min(val1, val2));
	}

	nova_std_decl(Max) {
		req_args(2);
		float val1 = std::get<float>(args[0]->data);
		float val2 = std::get<float>(args[1]->data);
		return Value(std::max(val1, val2));
	}

	nova_std_decl(Clamp) {
		req_args(3);
		float val = std::get<float>(args[0]->data);
		float min = std::get<float>(args[1]->data);
		float max = std::get<float>(args[2]->data);
		return Value(std::clamp(val, min, max));
	}

	nova_std_decl(Lerp) {
		req_args(3);
		float a = std::get<float>(args[0]->data);
		float b = std::get<float>(args[1]->data);
		float t = std::get<float>(args[2]->data);
		return Value(a + t * (b - a));
	}

	nova_std_decl(Degrees) {
		req_args(1);
		float radians = std::get<float>(args[0]->data);
		return Value(radians * (180.0f / 3.14159265358979323846f));
	}
	
	nova_std_decl(Radians) {
		req_args(1);
		float degrees = std::get<float>(args[0]->data);
		return Value(degrees * (3.14159265358979323846f / 180.0f));
	}

	nova_std_decl(Random) {
		req_args(0);
		return Value(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}

	nova_std_decl(RandomRange) {
		req_args(2);
		float min = std::get<float>(args[0]->data);
		float max = std::get<float>(args[1]->data);
		return Value(min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min))));
	}

	Scope GetModule() override {
		Scope scope;
		NOVA_BIND_METHOD(Sin);
		NOVA_BIND_METHOD(Cos);
		NOVA_BIND_METHOD(Tan);
		NOVA_BIND_METHOD(Sqrt);
		NOVA_BIND_METHOD(Abs);
		NOVA_BIND_METHOD(Round);
		NOVA_BIND_METHOD(Floor);
		NOVA_BIND_METHOD(Ceil);
		NOVA_BIND_METHOD(Min);
		NOVA_BIND_METHOD(Max);
		NOVA_BIND_METHOD(Clamp);
		NOVA_BIND_METHOD(Lerp);
		NOVA_BIND_METHOD(Degrees);
		NOVA_BIND_METHOD(Radians);
		NOVA_BIND_METHOD(Random);
		NOVA_BIND_METHOD(RandomRange);
		return scope;
	}

};

#endif