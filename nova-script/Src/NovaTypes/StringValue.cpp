#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/BoolValue.h"
#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Library/nova_std_macro.h"
#include "../NovaScript/NovaErrorPush.h"



NovaValue* NovaString::Copy() const {
	return new NovaString(str);
}

std::string NovaString::ToString() const {
	return str;
}

std::string NovaString::Type() const {
	return "String";
}

NovaValue* NovaString::PerformOp(NovaValue* rhs, const NovaOperator& op) const {
	if (Type() == "String") {
		NovaString* r = static_cast<NovaString*>(rhs);
		std::string result;
		switch(op) {
		case NovaOperator::Plus:
			result = str + r->str;
			break;
		case NovaOperator::Equality: {
			bool b = str == r->str;
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = str != r->str;
			return new NovaBool(b);
		}
		default:
			OpFailed(rhs, op);
			return nullptr;
			break;
		}

		return new NovaString(result);
	}

	OpFailed(rhs, op);
	return nullptr;
}

static void PushError(std::string msg) {
	Callbacker::PushError(msg, 2);
}

nova_std_decl(Length) {
	req_args(1);
	strget(string, 0);
	return new NovaInt(int(string->str.size()));
}

nova_std_decl(IsEmpty) {
	req_args(1);
	strget(string, 0);
	return new NovaBool(string->str.empty());
}

nova_std_decl(StartsWith) {
	req_args(2);
	strget(string, 0);
	strget(prefix, 1);
	return new NovaBool(string->str.starts_with(prefix->str));
}

nova_std_decl(EndsWith) {
	req_args(2);
	strget(string, 0);
	strget(suffix, 1);
	return new NovaBool(string->str.ends_with(suffix->str));
}

nova_std_decl(Contains) {
	req_args(2);
	strget(string, 0);
	strget(search, 1);
	return new NovaBool(string->str.find(search->str) != std::string::npos);
}

nova_std_decl(Substr) {
	req_args(3);
	strget(string, 0);
	intget(start, 1);
	intget(len, 2);

	if (start->num < 0 || len->num < 0 || start->num >= string->str.size()) {
		PushError("Invalid substring range");
		return new NovaString("");
	}

	return new NovaString(string->str.substr(start->num, len->num));
}

nova_std_decl(Find) {
	req_args(2);
	strget(string, 0);
	strget(search, 1);

	size_t pos = string->str.find(search->str);
	return new NovaInt(pos == std::string::npos ? -1 : int(pos));
}

// thanks stack overflow very cool
static std::string TrimImpl(const std::string& s) {
	size_t start = s.find_first_not_of(" \t\n\r");
	if (start == std::string::npos) return "";

	size_t end = s.find_last_not_of(" \t\n\r");
	return s.substr(start, end - start + 1);
}

nova_std_decl(Trim) {
	req_args(1);
	strget(string, 0);
	return new NovaString(TrimImpl(string->str));
}

nova_std_decl(Replace) {
	req_args(3);
	strget(string, 0);
	strget(from, 1);
	strget(to, 2);

	size_t pos = string->str.find(from->str);
	if (pos == std::string::npos) return new NovaString(string->str);

	std::string result = string->str;
	result.replace(pos, from->str.size(), to->str);
	return new NovaString(result);
}

nova_std_decl(ToUpper) {
	req_args(1);
	strget(string, 0);
	std::string out = string->str;
	for (char& c : out) {
		c = std::toupper((unsigned char)c);
	}

	return new NovaString(out);
}

nova_std_decl(ToLower) {
	req_args(1);
	strget(string, 0);
	std::string out = string->str;
	for (char& c : out) {
		c = std::tolower((unsigned char)c);
	}

	return new NovaString(out);
}

NovaString::NovaString(const std::string& str) : str(str) {
	accessables = new std::unordered_map<std::string, NovaValue*>;
	accessables->insert({ "Length", new NovaFunction(Length) });
	accessables->insert({ "IsEmpty", new NovaFunction(IsEmpty) });
	accessables->insert({ "StartsWith", new NovaFunction(StartsWith) });
	accessables->insert({ "EndsWith", new NovaFunction(EndsWith) });
	accessables->insert({ "Contains", new NovaFunction(Contains) });
	accessables->insert({ "Substr", new NovaFunction(Substr) });
	accessables->insert({ "Find", new NovaFunction(Find) });
	accessables->insert({ "Trim", new NovaFunction(Trim) });
	accessables->insert({ "Replace", new NovaFunction(Replace) });
	accessables->insert({ "ToUpper", new NovaFunction(ToUpper) });
	accessables->insert({ "ToLower", new NovaFunction(ToLower) });
	accessables->insert({ "this", nullptr });
}