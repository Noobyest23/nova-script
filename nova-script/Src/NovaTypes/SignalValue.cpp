#include "../NovaScript/Value/SignalValue.h"

static void PushError(std::string msg, int sev = 2) {
	Callbacker::PushError(msg, sev);
}

nova_std_decl(Emit) {
	req_args(1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0]);
	std::vector<NovaValue*> new_args;
	for (int i = 1; i < args.size(); i++) {
		new_args.push_back(args[i]);
	}

	for (NovaValue* val : signal->connections) {
		auto* func = static_cast<NovaFunction*>(val);
		func->Call(new_args);
	}
	return nullptr;
}

nova_std_decl(Connect) {
	req_args(2);
	funcget(func, 1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0]);
	signal->connections.push_back(func);
	func->AddRef();
	return nullptr;
}

nova_std_decl(Disconnect) {
	req_args(2);
	funcget(func, 1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0]);
	signal->connections.erase(std::remove(signal->connections.begin(), signal->connections.end(), func));
	func->Release();
	return nullptr;
}

nova_std_decl(GetConnections) {
	req_args(1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0]);
	return new NovaArray(signal->connections);
}

NovaSignal::NovaSignal() {
	accessables = new std::unordered_map<std::string, NovaValue*> {
		{"Emit", new NovaFunction(Emit, true)},
		{"Connect", new NovaFunction(Connect, true)},
		{"GetConnections", new NovaFunction(GetConnections, true)},
		{"Disconnect", new NovaFunction(Disconnect, true)},
		{"_NOVA_THIS", nullptr}
	};
}

void NovaSignal::OnDestroy() {
	for (NovaValue* f : connections) {
		f->Release();
	}
	delete this;
}