#ifndef NOVASCRIPT_PARSER_H
#define NOVASCRIPT_PARSER_H

#include "Tokens.h"
#include <vector>
#include "../ASTNodes/ProgramNode.h"
#include <unordered_map>
#include "../../NovaErrorPush.h"
class Interpretor;

class Parser {
public:

	Parser(std::vector<Token>& tokens, bool use_optimizations = true) : tokens(tokens), use_optimizations(use_optimizations) {};

	ProgramNode* Parse();


private:

	StmtNode* ParseStatement();
	ExprNode* ParseExpression();

	ExprNode* ParseTernary();
	ExprNode* ParseAccesor();
	ExprNode* ParseUnary();
	ExprNode* ParseAssignment();
	ExprNode* ParseBoolean();
	ExprNode* ParseEquality();
	ExprNode* ParseComparison();
	ExprNode* ParseArithmetic();
	ExprNode* ParseTerm();
	ExprNode* ParsePrimary();

	void Advance() { t_index++; };
	Token& Current() const { return tokens[t_index]; };
	bool Accept(NovaTokenType type) const { return (tokens[t_index].type == type); };
	bool AcceptNext(NovaTokenType type) const { return (Next().type == type); };
	Token Last() const { return tokens[t_index - 1]; };
	Token Next() const { return tokens[t_index + 1]; };
	unsigned int t_index = 0;
	std::vector<Token>& tokens;

	bool force_stop = false;
	bool soft_stop = false;

	bool current_expression_has_const = false;

	void PushError(const std::string& message);
	void PushWarning(const std::string& message);

	bool use_optimizations = true;

};



#endif
