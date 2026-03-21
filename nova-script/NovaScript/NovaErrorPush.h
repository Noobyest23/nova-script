#ifndef NOVA_SCRIPT_PUSH_ERROR
#define NOVA_SCRIPT_PUSH_ERROR
#include "NovaScript_API.h"
#include <string>

static bool _use_console = false;

namespace NovaInternal {
	extern void(*_PushErrorCallback)(const char* message, int sevarity);
	extern void(*_ExitCallback)(const char* message);
	extern std::string _proj_path;
	extern void(*_ProjectPathSetCallback)(const char* path);
}

class NOVASCRIPT_API Callbacker {
public:

	static void PushError(const std::string& message, int sevarity);
	static void ExitCallback(const std::string& message);
	static void ProjectSetCallback(const std::string& message);

	static void(*_PushErrorCallback)(const char* message, int sevarity);
	static void(*_ExitCallback)(const char* message);
	static std::string _proj_path;
	static void(*_ProjectPathSetCallback)(const char* path);
	
};

#endif