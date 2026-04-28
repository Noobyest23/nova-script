#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/BoolValue.h"
#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Value/ArrayValue.h"
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


std::string NovaString::ToString() const {
	return CStr();
}

std::string NovaString::Type() const {
	return "String";
}

std::shared_ptr<NovaValue> NovaString::Copy() const {
	return std::make_shared<NovaString>(CStr());
}

std::shared_ptr<NovaValue> NovaString::CopyPtr() {
	return std::make_shared<NovaString>(Str());
}

std::shared_ptr<NovaValue> NovaString::PerformOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) const {
	if (rhs->Type() == "String") {
		NovaString* r = static_cast<NovaString*>(rhs.get());
		std::string result;
		switch(op) {
		case NovaOperator::Plus:
			result = CStr() + r->CStr();
			break;
		case NovaOperator::Equality: {
			bool b = CStr() == r->CStr();
			return std::make_shared<NovaBool>(b);
		}
		case NovaOperator::NotEqual: {
			bool b = CStr() != r->CStr();
			return std::make_shared<NovaBool>(b);
		}
		default:
			return nullptr;
			break;
		}

		return std::make_shared<NovaString>(result);
	}

	return nullptr;
}


bool NovaString::PerformCompoundOp(std::shared_ptr<NovaValue> rhs, const NovaOperator& op) {
	if (Type() == "String") {
		NovaString* r = static_cast<NovaString*>(rhs.get());
		std::string result;
		switch (op) {
		case NovaOperator::CompoundPlus:
			result += r->CStr();
			return true;
		break;
		default:
			return false;
			break;
		}

		return new NovaString(result);
	}

	return false;
}

bool NovaString::Assign(std::shared_ptr<NovaValue> rhs) {
	if (rhs->Type() == "String") {
		NovaString* str = static_cast<NovaString*>(rhs.get());
		*Str() = str->CStr();
		return true;
	}
	false;
}

static void PushError(std::string msg) {
	Callbacker::PushError(msg, 2);
}

nova_std_decl(Length) {
	req_args(1);
	strget(string, 0);
	return std::make_shared<NovaBool>(int(string->CStr().size()));
}

nova_std_decl(IsEmpty) {
	req_args(1);
	strget(string, 0);
	return std::make_shared<NovaBool>(string->CStr().empty());
}

nova_std_decl(StartsWith) {
	req_args(2);
	strget(string, 0);
	strget(prefix, 1);
	return std::make_shared<NovaBool>(string->CStr().starts_with(prefix->CStr()));
}

nova_std_decl(EndsWith) {
	req_args(2);
	strget(string, 0);
	strget(suffix, 1);
	return std::make_shared<NovaBool>(string->CStr().ends_with(suffix->CStr()));
}

nova_std_decl(Contains) {
	req_args(2);
	strget(string, 0);
	strget(search, 1);
	return std::make_shared<NovaBool>(string->CStr().find(search->CStr()) != std::string::npos);
}

nova_std_decl(Substr) {
	req_args(3);
	strget(string, 0);
	intget(start, 1);
	intget(len, 2);

	if (start->CNum() < 0 || len->CNum() < 0 || start->CNum() >= string->CStr().size()) {
		PushError("Invalid substring range");
		return std::make_shared<NovaString>("");
	}

	return std::make_shared<NovaString>(string->CStr().substr(start->CNum(), len->CNum()));
}

nova_std_decl(Find) {
	req_args(2);
	strget(string, 0);
	strget(search, 1);

	size_t pos = string->CStr().find(search->CStr());
	return std::make_shared<NovaInt>(pos == std::string::npos ? -1 : int(pos));
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
	return std::make_shared<NovaString>(TrimImpl(string->CStr()));
}

nova_std_decl(Replace) {
	req_args(3);
	strget(string, 0);
	strget(from, 1);
	strget(to, 2);

	size_t pos = string->CStr().find(from->CStr());
	if (pos == std::string::npos) return std::make_shared<NovaString>(string->CStr());

	std::string result = string->CStr();
	result.replace(pos, from->CStr().size(), to->CStr());
	return std::make_shared<NovaString>(result);
}

nova_std_decl(ToUpper) {
	req_args(1);
	strget(string, 0);
	std::string out = string->CStr();
	for (char& c : out) {
		c = std::toupper((unsigned char)c);
	}

	return std::make_shared<NovaString>(out);
}

nova_std_decl(ToLower) {
	req_args(1);
	strget(string, 0);
	std::string out = string->CStr();
	for (char& c : out) {
		c = std::tolower((unsigned char)c);
	}

	return std::make_shared<NovaString>(out);
}

nova_std_decl(Split) {
	req_args(2);
	strget(string, 0);
	strget(delimiter, 1);

	std::string text = string->CStr();
	std::string delim = delimiter->CStr();

	std::vector<std::shared_ptr<NovaValue>> result;

	if (delim.empty()) {
		result.push_back(std::make_shared<NovaString>(text));
		return std::make_shared<NovaArray>(result);
	}

	size_t start = 0;
	size_t end = text.find(delim);

	while (end != std::string::npos) {
		result.push_back(std::make_shared<NovaString>(text.substr(start, end - start)));
		start = end + delim.length();
		end = text.find(delim, start);
	}

	result.push_back(std::make_shared<NovaString>(text.substr(start)));

	return std::make_shared<NovaArray>(result);
}

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaString::string_accessables = std::unordered_map<std::string, std::shared_ptr<NovaValue>>{
	{"Length", std::make_shared<NovaFunction>(Length, true)},
	{"IsEmpty", std::make_shared<NovaFunction>(IsEmpty, true)},
	{"StartsWith",std::make_shared<NovaFunction>(StartsWith, true)},
	{"EndsWith", std::make_shared<NovaFunction>(EndsWith, true)},
	{"Substr", std::make_shared<NovaFunction>(Substr, true)},
	{"Find", std::make_shared<NovaFunction>(Find, true)},
	{"Replace", std::make_shared<NovaFunction>(Replace, true)},
	{"ToUpper", std::make_shared<NovaFunction>(ToUpper, true)},
	{"ToLower", std::make_shared<NovaFunction>(ToLower, true)},
	{"Split", std::make_shared<NovaFunction>(Split, true)},
	{"_NOVA_THIS", nullptr }
};

NovaString::NovaString(const std::string& str) : novastr(str) {
	
}

NovaString::NovaString(std::string* cppstr) : cppstr(cppstr) {
	
}

std::unordered_map<std::string, std::shared_ptr<NovaValue>> NovaString::GetFullAccessableList() {
	std::unordered_map<std::string, std::shared_ptr<NovaValue>> map = NovaValue::GetFullAccessableList();

	for (std::pair<std::string, std::shared_ptr<NovaValue>> pair : string_accessables) {
		map[pair.first] = pair.second;
	}
	return map;
}
