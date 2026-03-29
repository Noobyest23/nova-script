#include "../NovaScript/Value/Value.h"
#include "../NovaScript/NovaErrorPush.h"

void NovaValue::AddRef() {
	ref_count++;
}

void NovaValue::Release() {
	ref_count--;
	if (ref_count <= 0) {
		OnDestroy();
		if (accessables) {
			for (std::pair<std::string, NovaValue*> pair : *accessables) {
				if (pair.second) {
					pair.second->Release();
				}
			}
			delete accessables;
		}
		delete this;
	}
}

void NovaValue::PushError(const std::string& msg) const {
	Callbacker::PushError(msg, 2);
}

void NovaValue::PushWarning(const std::string & msg) const {
	Callbacker::PushError(msg, 1);
}

void NovaValue::OpFailed(NovaValue* rhs, const NovaOperator& op) const {
	PushError(Type() + " failed to perform " + std::to_string(op) + " type operation with a value of " + rhs->Type());
}

NovaValue* NovaValue::Access(const std::string& str) {
	if (accessables) {
		auto it = accessables->find(str);
		if (it != accessables->end()) {
			return accessables->at(str);
		}
	}
	else {
		PushError("Cannot access " + str + ", this value has no accessable values");
		return nullptr;
	}
}