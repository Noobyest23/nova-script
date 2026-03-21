#ifndef NOVASCRIPT_LEXER_H
#define NOVASCRIPT_LEXER_H

#include <vector>
#include "Tokens.h"

class Lexer {
public:

	Lexer(const char* filepath);

	std::vector<Token> Parse();

private:

	void Advance();
	Token Number();
	Token Identifier();

	char Current() const;
	char Peek(int offset) const;

	int pos = 0;

	int line = 1;
	int column = 1;

	std::string text;

};

#endif