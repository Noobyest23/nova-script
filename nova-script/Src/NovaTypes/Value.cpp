#include "../NovaScript/Value/Value.h"
#include "../NovaScript/NovaErrorPush.h"
#include "../NovaScript/Library/nova_std_macro.h"
#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/FunctionValue.h"

static void PushError(const std::string& msg, int sev = 2) {
	Callbacker::PushError(msg, sev);
}

nova_std_decl(SType) {
	req_args(1);
	return std::make_shared<NovaString>(args[0]->Type());
}

nova_std_decl(SToString) {
	req_args(1);
	return std::make_shared<NovaString>(args[0]->ToString());
}

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaValue::default_static_accessables = {
	{"Type", std::make_shared<NovaFunction>(SType, true)},
	{"ToString", std::make_shared<NovaFunction>(SToString, true)},
	{"_NOVA_THIS", nullptr}
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