#define NOVASCRIPT_EXPORTS
#include "NovaErrorPush.h"

void(*Callbacker::_PushErrorCallback)(const char* message, int sevarity) = nullptr;
void(*Callbacker::_ExitCallback)(const char* message) = nullptr;
std::string Callbacker::_proj_path = "";
void(*Callbacker::_ProjectPathSetCallback)(const char* path) = nullptr;

void Callbacker::PushError(const std::string& message, int sevarity) {
	if (_PushErrorCallback) {
		_PushErrorCallback(message.c_str(), sevarity);
	}
}

void Callbacker::ExitCallback(const std::string& message) {
	if (_ExitCallback) {
		_ExitCallback(message.c_str());
	}
}

void Callbacker::ProjectSetCallback(const std::string& path) {
	if (_ProjectPathSetCallback) {
		_ProjectPathSetCallback(path.c_str());
	}
	_proj_path = path;
}