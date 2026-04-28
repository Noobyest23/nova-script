#include "../NovaScript/Value/SignalValue.h"

static void PushError(std::string msg, int sev = 2) {
	Callbacker::PushError(msg, sev);
}

nova_std_decl(Emit) {
	req_args(1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0].get());
	std::vector<std::shared_ptr<NovaValue>> new_args;
	for (int i = 1; i < args.size(); i++) {
		new_args.push_back(args[i]);
	}

	for (std::shared_ptr<NovaValue> val : signal->connections) {
		auto* func = static_cast<NovaFunction*>(val.get());
		func->Call(new_args);
	}
	return nullptr;
}

nova_std_decl(Connect) {
	req_args(2);
	funcget(f, 1)
	NovaSignal* signal = static_cast<NovaSignal*>(args[0].get());
	signal->connections.push_back(std::make_shared<NovaFunction>(*f));
	return nullptr;
}

nova_std_decl(Disconnect) {
	req_args(2);
	funcget(func, 1);

	NovaSignal* signal = static_cast<NovaSignal*>(args[0].get());

	auto it = std::remove_if(signal->connections.begin(),
		signal->connections.end(),
		[func](const std::shared_ptr<NovaFunction>& connected_ptr) {
			return connected_ptr.get() == func;
		});

	signal->connections.erase(it, signal->connections.end());

	return nullptr;
}

nova_std_decl(GetConnections) {
	req_args(1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0].get());
	std::vector<std::shared_ptr<NovaValue>> base_connections;
	for (auto c : signal->connections) {
		base_connections.push_back(c->Copy());
	}
	return std::make_shared<NovaArray>(base_connections);
}

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaSignal::signal_accessables = {
	{"Emit", std::make_shared<NovaFunction>(Emit, true)},
	{"Connect", std::make_shared<NovaFunction>(Connect, true)},
	{"Disconnect", std::make_shared<NovaFunction>(Disconnect, true)},
	{"GetConnections", std::make_shared<NovaFunction>(GetConnections, true)},
	{"_NOVA_THIS", nullptr }
};

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaSignal::GetFullAccessableList() {
	std::unordered_map<std::string, std::shared_ptr<NovaValue>> map = NovaValue::GetFullAccessableList();

	for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : signal_accessables) {
		map[pair.first] = pair.second;
	}

	return map;
}