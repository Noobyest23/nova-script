#ifndef NOVASCRIPT_ENGINE_H
#define NOVASCRIPT_ENGINE_H
/*
namespace nova_std_engine {
	// functions begining with window expect the window as their first argument
	static void PushError(const std::string& message) {
		Engine* engine = Engine::GetInstance();
		engine->PushError("[NovaScript Engine Library] " + message);
	}

	static nova_std_decl(window_get_size) {
		req_args(1);
		objget(window, Window, 0);
		Value size = (window->GetSize());
		return size;
	}
	static nova_std_decl(window_get_name) {
		req_args(1);
		objget(window, Window, 0);
		Value name = Value(window->GetName());
		return name;
	}
	static nova_std_decl(window_set_size) {
		req_args(2);
		objget(window, Window, 0);
		glm::vec2 size = std::get<glm::vec2>(args[1]->data);
		window->SetSize(size);
		return null_value;
	}
	static nova_std_decl(window_set_name) {
		req_args(2);
		objget(window, Window, 0);
		std::string name = args[1]->GetString();
		window->SetName(name);
		return null_value;
	}
	static nova_std_decl(GetWindow) {
		CPPObject WindowType;
		WindowType.ptr = Engine::GetInstance()->window;
		WindowType.scope.Set("GetSize", window_get_size);
		WindowType.scope.Set("SetSize", window_set_size);
		WindowType.scope.Set("SetName", window_set_name);
		WindowType.scope.Set("GetName", window_get_name);
		return WindowType;
	}

	static nova_std_decl(InitProjectPath) {
		req_args(1);
		strget(string, 0);
		if (!string.ends_with("/") or !string.ends_with("\\")) {
			string.append("/");
		}
		Engine* engine = Engine::GetInstance();
		engine->InitProjectPath(string);
		return null_value;
	}

	static nova_std_decl(Exit) {
		std::string output = "[NovaScript] ";
		for (Value* arg : args) {
			output += arg->ToString(); if (arg != args.back()) { output += ", "; };
		}
		Engine* engine = Engine::GetInstance();
		engine->PushError(output, true);
		return null_value;
	}

	static Value get_module() {
		Engine* engine = Engine::GetInstance();
		Scope scope;
		scope.Set("Exit", Exit);
		scope.Set("GetWindow", GetWindow);
		scope.Set("SupressWarningPopup", Value(CPPVariable(engine->suppress_warning_popup)));
		scope.Set("SupressErrorPopup", Value(CPPVariable(engine->suppress_error_popup)));
		scope.Set("InitProjectPath", InitProjectPath);
		return scope;
	}
	
};
*/
#endif