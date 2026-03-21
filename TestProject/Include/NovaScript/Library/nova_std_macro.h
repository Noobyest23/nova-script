#ifndef NOVASCRIPT_STD_MACROS
#define NOVASCRIPT_STD_MACROS

#include "../Interpretor/Value.h"

static Value null_value = Value();

#define nova_std_decl(name) static Value name(std::vector<Value*>& args)
#define req_args(num) if (args.size() < num) { PushError("Expected " + std::string(#num) + " args in function call"); return Value(); }

#define strget(str, arg_i) if (!args[arg_i]->IsString()) { PushError("Expected string as argument " + std::to_string(arg_i)); return Value(); } std::string& str = args[arg_i]->GetString();
#define intget(i, arg_i) if (!args[arg_i]->IsInt()) { PushError("Expected int as argument " + std::to_string(arg_i)); return Value(); } int& i = args[arg_i]->GetInt();
#define floatget(f, arg_i) if (!args[arg_i]->IsFloat()) { PushError("Expected float as argument " + std::to_string(arg_i)); return Value(); } float& f = args[arg_i]->GetFloat();
#define vec2get(v, arg_i) if (!args[arg_i]->IsVec2()) {PushError("Expected Vector2 as argument " + std::to_string(arg_i)); return Value(); } glm::vec2& v = args[arg_i]->GetVec2();
#define vec3get(v, arg_i) if (!args[arg_i]->IsVec3()) {PushError("Expected Vector3 as argument " + std::to_string(arg_i)); return Value(); } glm::vec3& v = args[arg_i]->GetVec3();
#define vec4get(v, arg_i) if (!args[arg_i]->IsVec4()) {PushError("Expected Vector4 as argument " + std::to_string(arg_i)); return Value(); } glm::vec4& v = args[arg_i]->GetVec4();

template<typename T>
T* ObjGet(std::vector<Value*>&args, int i) {
	CPPObject& obj = std::get<CPPObject>(args[i]->data);
	return static_cast<T*>(obj.ptr);
}

#define objget(obj, type, arg_i) type* obj = ObjGet<type>(args, arg_i);

#define NOVA_VOID_RETURN(cls, method) \
nova_std_decl(method) { \
	req_args(1); \
	objget(obj, cls, 0); \
	obj->method(); \
return null_value; \
}

#define NOVA_GETTER(cls, method) \
nova_std_decl(method) { \
	req_args(1); \
	objget(obj, cls, 0); \
	return Value(obj->method()); \
}

#define NOVA_SETTER(cls, method, a_1_get_prefix) \
nova_std_decl(method) { \
	req_args(2); \
	objget(obj, cls, 0); \
	a_1_get_prefix##get(arg1, 1); \
	obj->method(arg1); \
	return null_value; \
}

#define NOVA_GETTER_TYPE_WRAP(cls, method, type) \
nova_std_decl(method) { \
	req_args(1); \
	objget(obj, cls, 0); \
	return Value(type(obj->method())); \
}



#define NOVA_BIND_PROPERTY(prop) scope.Set(#prop, Value(CPPVariable(std::reference_wrapper<decltype(prop)>(prop))))
#define NOVA_BIND_METHOD(method) scope.Set(#method, method);
#define NOVA_BIND_WHOLE_NAMESPACE(space) Scope s = space##::GetModule(); \
for (std::pair<std::string, Value> pair : s.GetAll()) { \
	scope.Set(pair.first, pair.second); \
}

#define NOVA_RETURN_FUNC_ARG1(cls, method, a_1_get_prefix) \
nova_std_decl(method) { \
	req_args(2); \
	objget(obj, cls, 0); \
	a_1_get_prefix##get(arg1, 1); \
	return Value(obj->method(arg1)); \
}

#define NOVA_ERR_PUSHER(cls) static void PushError(const std::string& msg) { \
		Engine::GetInstance()->PushError(std::string("[") + #cls + std::string("]") + msg); \
}

#define NOVA_ASSET_OBJ_SETTER(cls, method, ocls) \
nova_std_decl(method) { \
req_args(2); \
objget(obj, cls, 0); \
objget(arg1, ocls, 1); \
obj->method(arg1); \
return null_value; \
}


#endif