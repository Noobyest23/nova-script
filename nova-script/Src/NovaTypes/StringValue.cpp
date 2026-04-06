#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/BoolValue.h"
#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Library/nova_std_macro.h"
#include "../NovaScript/NovaErrorPush.h"

std::string* NovaString::Str() {
	if (cppstr) {
		return cppstr;
	}
	return &novastr;
}

const std::string& NovaString::CStr() const {
	if (cppstr) {
		return *cppstr;
	}
	return novastr;
}

NovaValue* NovaString::Copy() {
	return new NovaString(CStr());
}

std::string NovaString::ToString() const {
	return CStr();
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
			result = CStr() + r->CStr();
			break;
		case NovaOperator::Equality: {
			bool b = CStr() == r->CStr();
			return new NovaBool(b);
		}
		case NovaOperator::NotEqual: {
			bool b = CStr() != r->CStr();
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


NovaValue* NovaString::PerformCompoundOp(NovaValue* rhs, const NovaOperator& op) {
	if (Type() == "String") {
		NovaString* r = static_cast<NovaString*>(rhs);
		std::string result;
		switch (op) {
		case NovaOperator::CompoundPlus:
			result += r->CStr();
			return this;
		break;
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

NovaValue* NovaString::Assign(NovaValue* rhs) {
	if (rhs->Type() == "String") {
		NovaString* str = static_cast<NovaString*>(rhs);
		*Str() = str->CStr();
		return this;
	}
	return nullptr;
}

static void PushError(std::string msg) {
	Callbacker::PushError(msg, 2);
}

nova_std_decl(Length) {
	req_args(1);
	strget(string, 0);
	return new NovaInt(int(string->CStr().size()));
}

nova_std_decl(IsEmpty) {
	req_args(1);
	strget(string, 0);
	return new NovaBool(string->CStr().empty());
}

nova_std_decl(StartsWith) {
	req_args(2);
	strget(string, 0);
	strget(prefix, 1);
	return new NovaBool(string->CStr().starts_with(prefix->CStr()));
}

nova_std_decl(EndsWith) {
	req_args(2);
	strget(string, 0);
	strget(suffix, 1);
	return new NovaBool(string->CStr().ends_with(suffix->CStr()));
}

nova_std_decl(Contains) {
	req_args(2);
	strget(string, 0);
	strget(search, 1);
	return new NovaBool(string->CStr().find(search->CStr()) != std::string::npos);
}

nova_std_decl(Substr) {
	req_args(3);
	strget(string, 0);
	intget(start, 1);
	intget(len, 2);

	if (start->CNum() < 0 || len->CNum() < 0 || start->CNum() >= string->CStr().size()) {
		PushError("Invalid substring range");
		return new NovaString("");
	}

	return new NovaString(string->CStr().substr(start->CNum(), len->CNum()));
}

nova_std_decl(Find) {
	req_args(2);
	strget(string, 0);
	strget(search, 1);

	size_t pos = string->CStr().find(search->CStr());
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
	return new NovaString(TrimImpl(string->CStr()));
}

nova_std_decl(Replace) {
	req_args(3);
	strget(string, 0);
	strget(from, 1);
	strget(to, 2);

	size_t pos = string->CStr().find(from->CStr());
	if (pos == std::string::npos) return new NovaString(string->CStr());

	std::string result = string->CStr();
	result.replace(pos, from->CStr().size(), to->CStr());
	return new NovaString(result);
}

nova_std_decl(ToUpper) {
	req_args(1);
	strget(string, 0);
	std::string out = string->CStr();
	for (char& c : out) {
		c = std::toupper((unsigned char)c);
	}

	return new NovaString(out);
}

nova_std_decl(ToLower) {
	req_args(1);
	strget(string, 0);
	std::string out = string->CStr();
	for (char& c : out) {
		c = std::tolower((unsigned char)c);
	}

	return new NovaString(out);
}

NovaString::NovaString(const std::string& str) : novastr(str) {
	accessables = new std::unordered_map<std::string, NovaValue*>{
	{"Length", new NovaFunction(Length)},
	{"IsEmpty", new NovaFunction(IsEmpty)},
	{"StartsWith", new NovaFunction(StartsWith)},
	{"EndsWith", new NovaFunction(EndsWith)},
	{"Contains", new NovaFunction(Contains)},
	{"Substr", new NovaFunction(Substr)},
	{"Find", new NovaFunction(Find)},
	{"Trim", new NovaFunction(Trim)},
	{"Replace", new NovaFunction(Replace)},
	{"ToUpper", new NovaFunction(ToUpper)},
	{"ToLower", new NovaFunction(ToLower)},
	{"this", nullptr}
	};
}

NovaString::NovaString(std::string* cppstr) : cppstr(cppstr) {
	accessables = new std::unordered_map<std::string, NovaValue*>{
	{"Length", new NovaFunction(Length)},
	{"IsEmpty", new NovaFunction(IsEmpty)},
	{"StartsWith", new NovaFunction(StartsWith)},
	{"EndsWith", new NovaFunction(EndsWith)},
	{"Contains", new NovaFunction(Contains)},
	{"Substr", new NovaFunction(Substr)},
	{"Find", new NovaFunction(Find)},
	{"Trim", new NovaFunction(Trim)},
	{"Replace", new NovaFunction(Replace)},
	{"ToUpper", new NovaFunction(ToUpper)},
	{"ToLower", new NovaFunction(ToLower)},
	{"this", nullptr}
	};
}

void NovaString::OnDestroy() {
	delete this;
}
