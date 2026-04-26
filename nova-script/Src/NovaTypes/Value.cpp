#include "../NovaScript/Value/Value.h"
#include "../NovaScript/NovaErrorPush.h"

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaValue::default_static_accessables = {

};

void NovaValue::OnDestroy() {
	on_destroy();
	if (accessables) {
		delete accessables;
	}
}

void NovaValue::PushError(const std::string& msg) const {
	Callbacker::PushError(msg, 2);
}

void NovaValue::PushWarning(const std::string & msg) const {
	Callbacker::PushError(msg, 1);
}

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaValue::GetFullAccessableList() {
	if (accessables) {
		std::unordered_map<std::string, std::shared_ptr<NovaValue>> map;
		for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : default_static_accessables) {
			map[pair.first] = pair.second;
		}
	}
	return default_static_accessables;
}