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
}

nova_std_decl(Connect) {
	req_args(2);
	funcget(func, 1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0]);
	signal->connections.push_back(func);
}

nova_std_decl(GetConnections) {
	req_args(1);
	NovaSignal* signal = static_cast<NovaSignal*>(args[0]);
	return new NovaArray(signal->connections);
}

NovaSignal::NovaSignal() {
	accessables = new std::unordered_map<std::string, NovaValue*>{
		{"Emit", new NovaFunction(Emit)},
		{"Connect", new NovaFunction(Connect)},
		{"GetConnections", new NovaFunction(GetConnections)},
		{"this", nullptr}
	};
}