#ifndef NOVA_SCRIPT_MATH_H
#define NOVA_SCRIPT_MATH_H
#include "NovaModule.h"
#include <algorithm>
#include "../Value/FloatValue.h"
#include "../Value/IntValue.h"
#include "nova_std_macro.h"
#undef min
#undef max

#include <random>

// Static random engine to keep state between calls
static std::mt19937& GetRandEngine() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}

static std::shared_ptr<NovaValue> nova_float_to_int(std::shared_ptr<NovaValue> f) {
	if (f->Type() == "Float") {
		NovaFloat* flt = static_cast<NovaFloat*>(f.get());
		float rhs = flt->CNum();
		if (std::fmod(rhs, 1.0) == 0) {
			return std::make_shared<NovaInt>(int(rhs));
		}
	}
	return f;
}

class NovaMathModule : public NovaModule {

	

	// --- Trig & Basic Arithmetic ---
	nova_std_decl(Sin) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(sin(val)));
	}

	nova_std_decl(Cos) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(cos(val)));
	}

	nova_std_decl(Tan) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(tan(val)));
	}

	nova_std_decl(Sqrt) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(sqrt(val)));
	}

	nova_std_decl(Abs) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(abs(val)));
	}

	// --- Rounding ---
	nova_std_decl(Round) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(round(val)));
	}

	nova_std_decl(Floor) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(floor(val)));
	}

	nova_std_decl(Ceil) {
		req_args(1); numfget(val, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(ceil(val)));
	}

	// --- Comparison & Interpolation ---
	nova_std_decl(Min) {
		req_args(2);
		numfget(v1, 0);
		numfget(v2, 1);
		return nova_float_to_int(std::make_shared<NovaFloat>(std::min(v1, v2)));
	}

	nova_std_decl(Max) {
		req_args(2);
		numfget(v1, 0);
		numfget(v2, 1);
		return nova_float_to_int(std::make_shared<NovaFloat>(std::max(v1, v2)));
	}

	nova_std_decl(Clamp) {
		req_args(3); numfget(v, 0); numfget(lo, 1); numfget(hi, 2);
		return nova_float_to_int(std::make_shared<NovaFloat>(std::clamp(v, lo, hi)));
	}

	nova_std_decl(Lerp) {
		req_args(3); numfget(a, 0); numfget(b, 1); numfget(t, 2);
		return nova_float_to_int(std::make_shared<NovaFloat>(a + t * (b - a)));
	}

	// --- Conversion ---
	nova_std_decl(Degrees) {
		req_args(1); numfget(rad, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(rad * (180.0f / 3.1415926535f)));
	}

	nova_std_decl(Radians) {
		req_args(1); numfget(deg, 0);
		return nova_float_to_int(std::make_shared<NovaFloat>(deg * (3.1415926535f / 180.0f)));
	}

	// --- Randomization ---
	nova_std_decl(RandF) {
		req_args(0);
		std::uniform_real_distribution<float> dis(0.0f, 1.0f);
		return nova_float_to_int(std::make_shared<NovaFloat>(dis(GetRandEngine())));
	}

	nova_std_decl(RandI) {
		req_args(0);
		std::uniform_int_distribution<int> dis(0, std::numeric_limits<int>::max());
		return std::make_shared<NovaInt>(dis(GetRandEngine()));
	}

	nova_std_decl(RandFRange) {
		req_args(2);
		numfget(minV, 0);
		numfget(maxV, 1);
		std::uniform_real_distribution<float> dis(minV, maxV);
		return nova_float_to_int(std::make_shared<NovaFloat>(dis(GetRandEngine())));
	}

	nova_std_decl(RandIRange) {
		req_args(2);
		numfget(minV, 0);
		numfget(maxV, 1);
		std::uniform_int_distribution<int> dis(static_cast<int>(minV), static_cast<int>(maxV));
		return std::make_shared<NovaInt>(dis(GetRandEngine()));
	}

	std::shared_ptr<NovaObject> GetModule() override {
		std::shared_ptr<NovaObject> obj = std::make_shared<NovaObject>();

		objbindmethod(obj, Sin);
		objbindmethod(obj, Cos);
		objbindmethod(obj, Tan);
		objbindmethod(obj, Sqrt);
		objbindmethod(obj, Abs);
		objbindmethod(obj, Round);
		objbindmethod(obj, Floor);
		objbindmethod(obj, Ceil);
		objbindmethod(obj, Min);
		objbindmethod(obj, Max);
		objbindmethod(obj, Clamp);
		objbindmethod(obj, Lerp);
		objbindmethod(obj, Degrees);
		objbindmethod(obj, Radians);
		objbindmethod(obj, RandF);
		objbindmethod(obj, RandI);
		objbindmethod(obj, RandFRange);
		objbindmethod(obj, RandIRange);
		obj->PushBack("PI", std::make_shared<NovaFloat>(3.1415926535f));
		obj->PushBack("E", std::make_shared<NovaFloat>(2.7182818284f));
		return obj;
	}
};



#endif