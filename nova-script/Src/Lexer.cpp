#include "../NovaScript/Parser/Lexer.h"

#include <iostream>
#include <fstream>
#include "../NovaScript/NovaErrorPush.h"

Lexer::Lexer(const char* filepath) {
	
	std::ifstream file(filepath);
	std::string line;
	
	if (not file.is_open()) {
		Callbacker::PushError(("[Lexer] Failed to open file " + std::string(filepath)).c_str(), 2);
		return;
	}

	while (std::getline(file, line)) {
		text += line + "\n";
	}
}

std::vector<Token> Lexer::Parse() {
	std::vector<Token> result;

	while (pos < text.length()) {
		char c = Current();

		//if (c == '\n') {
		//	result.push_back({ NovaTokenType::NewLine, "\n", line, column});
		//}

		// Skip whitespace
		if (isspace(c)) {
			Advance();
			continue;
		}

		// Number literal
		if (isdigit(c)) {
			result.push_back(Number());
			continue;
		}

		// Identifier (variables, keywords later)
		if (isalpha(c) || c == '_') {
			result.push_back(Identifier());
			continue;
		}

		switch (c) {
		case '+':
			Advance();
			if (Current() == '=') {
				result.push_back({ NovaTokenType::CPlusOp, "+=", line, column});
				Advance();
			}
			else {
				result.push_back({ NovaTokenType::PlusOp, "+", line, column });
			}
			continue;
		case '-':
			Advance();
			if (Current() == '=') {
				result.push_back({ NovaTokenType::CMinusOp, "-=", line, column });
				Advance();
			}
			else {
				result.push_back({ NovaTokenType::MinusOp, "-", line, column });
			}
			continue;
		case '*':
			Advance();
			if (Current() == '=') {
				result.push_back({ NovaTokenType::CMultOp, "*=", line, column });
				Advance();
			}
			else {
				result.push_back({ NovaTokenType::MultOp, "*", line, column });
			}
			continue;
		case '/':
			Advance();
			if (Current() == '=') {
				result.push_back({ NovaTokenType::CDivOp, "/=", line, column });
				Advance();
			}
			else {
				result.push_back({ NovaTokenType::DivOp, "/", line, column });
			}
			continue;
		case '=':
			Advance();
			if (Current() == '=') {
				result.push_back({ NovaTokenType::EqualOp, "==", line, column });
				Advance();
			}
			else {
				result.push_back({ NovaTokenType::Assignment, "=", line, column });
			}
			continue;
		case '&':
			Advance();
			if (Current() == '&') {
				result.push_back({ NovaTokenType::AndOp, "&&", line, column });
			}
			continue;
		case '|':
			Advance();
			if (Current() == '|') {
				Advance();
				result.push_back({ NovaTokenType::OrOp, "||", line, column });
			}
			else if (Current() == '!') {
				Advance();
				result.push_back({ NovaTokenType::ASTPrint, "|~", line, column });
			}
			else {
				result.push_back({ NovaTokenType::BreakPoint, "|", line, column });
			}
			continue;
		case '(':
			Advance();
			result.push_back({ NovaTokenType::OpenParen, "(", line, column });
			continue;
		case ')':
			Advance();
			result.push_back({ NovaTokenType::CloseParen, ")", line, column });
			continue;
		case '{':
			Advance();
			result.push_back({ NovaTokenType::OpenBrace, "{", line, column });
			continue;
		case '}':
			Advance();
			result.push_back({ NovaTokenType::CloseBrace, "}", line, column });
			continue;
		case '>':
			Advance();
			if (Current() == '=') {
				Advance();
				result.push_back({ NovaTokenType::GtetOp, ">=", line, column });
			}
			else {
				result.push_back({ NovaTokenType::GreaterOp, ">", line, column });
			}
			continue;
		case '<':
			Advance();
			if (Current() == '=') {
				Advance();
				result.push_back({ NovaTokenType::LtetOp, "<=", line, column });
			}
			else {
				result.push_back({ NovaTokenType::LesserOp, "<", line, column });
			}
			continue;
		case ':':
			Advance();
			result.push_back({ NovaTokenType::Mirror, ":", line, column });
			continue;
		case '#':
			while (Current() != '\n') {
				Advance();
			}
			Advance();
			continue;
		case '"':
			Advance();
			{
				std::string content;
				while (Current() != '"') {
					content += Current();
					Advance();
				}
				Advance();
				result.push_back({ NovaTokenType::StringLit, content, line, column });
			}
			continue;
		case '[':
			Advance();
			result.push_back({ NovaTokenType::OpenBracket, "[", line, column});
			continue;
		case ']':
			Advance();
			result.push_back({ NovaTokenType::CloseBracket, "]", line, column});
			continue;
		case '.':
			Advance();
			result.push_back({ NovaTokenType::Dot, ".", line, column });
			continue;
		case '!':
			Advance();
			result.push_back({ NovaTokenType::Not, "!", line, column });
			continue;
		}

		Advance();
	}

	result.push_back({ NovaTokenType::Eof, "bye bye", line, column });
	return result;
}

void Lexer::Advance() {
	pos++;
	column++;
	if (Current() == '\n') {
		line++;
		column = 0;
	}
}

Token Lexer::Number() {
	std::string value;
	bool hasDot = false;

	while (true) {
		char c = Current();

		if (isdigit(c)) {
			value += c;
		}
		else if (c == '.' && !hasDot) {
			hasDot = true;
			value += c;
		}
		else {
			break;
		}

		Advance();
	}

	return Token{ hasDot ? NovaTokenType::FloatLit : NovaTokenType::IntLit, value, line, column };
}

Token Lexer::Identifier() {
	std::string value;

	while (isalnum(Current()) || Current() == '_') {
		value += Current();
		Advance();
	}
	if (value == "var") {
		return { NovaTokenType::VarDecl, value, line, column };
	}
	else if (value == "func") {
		return { NovaTokenType::FuncDecl, value, line, column };
	}
	else if (value == "if") {
		return { NovaTokenType::IfStmt, value, line, column };
	}
	else if (value == "else") {
		return { NovaTokenType::ElseStmt, value, line, column };
	}
	else if (value == "elif") {
		return { NovaTokenType::ElifStmt, value, line, column };
	}
	else if (value == "and") {
		return { NovaTokenType::AndOp, "&&", line, column };
	}
	else if (value == "or") {
		return { NovaTokenType::OrOp, "||", line, column };
	}
	else if (value == "type") {
		return { NovaTokenType::TypeDecl, value, line, column };
	}
	else if (value == "include") {
		return { NovaTokenType::Include, value, line, column };
	}
	else if (value == "return") {
		return { NovaTokenType::Return, value, line, column };
	}
	else if (value == "break") {
		return { NovaTokenType::Break, value, line, column };
	}
	else if (value == "continue") {
		return { NovaTokenType::Continue, value, line, column };
	}
	else if (value == "Vector2") {
		return { NovaTokenType::VectorLit2, value, line, column };
	}
	else if (value == "Vector3") {
		return { NovaTokenType::VectorLit3, value, line, column };
	}
	else if (value == "Vector4") {
		return { NovaTokenType::VectorLit4, value, line, column };
	}
	else if (value == "true") {
		return { NovaTokenType::BoolLit, value, line, column };
	}
	else if (value == "false") {
		return { NovaTokenType::BoolLit, value, line, column };
	}
	else if (value == "const") {
		return { NovaTokenType::Const, value, line, column };
	}
	else if (value == "for") {
		return { NovaTokenType::For, value, line, column };
	}
	else if (value == "while") {
		return { NovaTokenType::While, value, line, column };
	}
	else if (value == "in") {
		return { NovaTokenType::In, value, line, column };
	}
	else if (value == "null") {
		return { NovaTokenType::Null, value, line, column };
	}
	else if (value == "not") {
		return { NovaTokenType::Not, value, line, column };
	}
	return Token{ NovaTokenType::Identifier, value, line, column };
}

char Lexer::Current() const {
	return pos < text.length() ? text[pos] : '\0';
}

char Lexer::Peek(int offset = 1) const {
	size_t index = pos + offset;
	return index < text.length() ? text[index] : '\0';
}