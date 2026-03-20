#include "NovaModule.h"

void NovaModule::PushError(const std::string& message) {
	Callbacker::PushError(message.c_str(), 2);
};