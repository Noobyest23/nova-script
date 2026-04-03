#include "../NovaScript/Parser/Parser.h"

// TODO:
// Eric stupid he realise you can use switch on 

ProgramNode* Parser::Parse() {
	std::vector<StmtNode*> statements;
	while (t_index < tokens.size() - 1 and not force_stop and Current().type != NovaTokenType::Eof) {
		statements.push_back(ParseStatement());
	}
	ProgramNode* program = new ProgramNode(statements);
	program->valid_program = not force_stop;
	return program;
}

StmtNode* Parser::ParseStatement() {
	// Variable Declaration Node
	bool is_const = false;
	bool handled_const = false;
	if (Accept(NovaTokenType::Const)) {
		is_const = true;
		Advance();
	}
	if (Accept(NovaTokenType::VarDecl)) {
		// Next token is an identifier
		Advance();
		if (Accept(NovaTokenType::Identifier)) {
			std::string var_name = Current().content;
			Advance();

			// Optionally check for initialization
			ExprNode* expression = nullptr;
			if (Accept(NovaTokenType::Assignment)) {
				Advance();
				expression = ParseExpression();
			}
			else {
				PushWarning("Uninitialized Variable");
			}
			VarDeclNode* decl = new VarDeclNode(var_name, expression);
			decl->constant = is_const;
			return decl;
		}
		else {
			PushError("Expected variable name");
		}
	}
	// Function Declaration Node
	if (Accept(NovaTokenType::FuncDecl)) {
		Advance();
		if (Accept(NovaTokenType::Identifier)) {
			std::string func_name = Current().content;
			std::vector<std::string> args;
			std::vector<StmtNode*> body;
			Advance();
			
			if (Accept(NovaTokenType::OpenParen)) {
				Advance();
				while (!Accept(NovaTokenType::CloseParen)) {
					if (Accept(NovaTokenType::Identifier)) {
						args.push_back(Current().content);
						Advance();
					}
					else {
						PushError("Expected identifier");
						break;
					}

				}
				Advance();
				if (Accept(NovaTokenType::OpenBrace)) {
					Advance();
					while (!Accept(NovaTokenType::CloseBrace)) {
						body.push_back(ParseStatement());
					}
				}
				else {
					PushError("Expected {");
				}
				Advance();

				return new FuncDeclNode(func_name, args, body);
			}
			else {
				PushError("Expected (");
			}

		}
		else {
			PushError("Expected function name");
		}
	}
	// If Statement
	if (Accept(NovaTokenType::IfStmt)) {
		Advance();
		// Parenthesis optional on if statements
		if (Accept(NovaTokenType::OpenParen)) {
			Advance();
		}
		ExprNode* expression = ParseExpression();
		if (Accept(NovaTokenType::CloseParen)) {
			Advance();
		}

		if (Accept(NovaTokenType::OpenBrace)) {
			std::vector<StmtNode*> body;
			std::vector<StmtNode*> else_body;
			Advance();
			while (!Accept(NovaTokenType::CloseBrace)) {
				StmtNode* statement = ParseStatement();
				body.push_back(statement);
			}
			Advance();
			if (Accept(NovaTokenType::ElseStmt)) {
				Advance();
				if (Accept(NovaTokenType::OpenBrace)) {
					Advance();
					while (!Accept(NovaTokenType::CloseBrace)) {
						StmtNode* statement = ParseStatement();
						else_body.push_back(statement);
					}
					Advance();
				}
				else {
					PushError("Expected {");
				}
				
			}
			return new IfStmtNode(expression, body, else_body);
		}
		else {
			PushError("Expected {");
		}


	}
	// Type Declaration Node
	if (Accept(NovaTokenType::TypeDecl)) {
		Advance();
		if (Accept(NovaTokenType::Identifier)) {
			std::string type_id = Current().content;
			std::string mirror_id = "";
			Advance();
			// Optional inheritence
			if (Accept(NovaTokenType::Mirror)) {
				Advance();
				if (Accept(NovaTokenType::Identifier)) {
					mirror_id = Current().content;
					Advance();
				}
			}
			std::vector<StmtNode*> scope;
			if (Accept(NovaTokenType::OpenBrace)) {
				Advance();
				while (!Accept(NovaTokenType::CloseBrace)) {
					StmtNode* stmt = ParseStatement();
					scope.push_back(stmt);
				}
				Advance();

				return new TypeDeclNode(type_id, mirror_id, scope);
			}
			else {
				PushError("Expected {");
			}

		}
		else {
			PushError("Expected an identifier");
		}
	}
	// Include Statement
	if (Accept(NovaTokenType::Include)) {
		Advance();
		ExprNode* expr = ParseExpression();
		if (expr) {
			if (Accept(NovaTokenType::Identifier) and Current().content == "as") {
				Advance();
				ExprNode* as = ParseExpression();
				return new IncludeNode(expr, as);
			}
			return new IncludeNode(expr);
		}
		else {
			PushError("Expected an expression");
		}
	}
	if (Accept(NovaTokenType::Return)) {
		Advance();
		ExprNode* return_value = ParseExpression();
		if (return_value) {
			return new ReturnStmtNode(return_value);
		}
		else {
			PushError("Expected an expression");
		}
	}
	if (Accept(NovaTokenType::BreakPoint)) {
		Advance();
		StmtNode* stmt = ParseStatement();
		return new BreakPointNode(stmt);
	}
	if (Accept(NovaTokenType::ASTPrint)) {
		Advance();
		return new ASTPrintNode;
	}
	if (Accept(NovaTokenType::For)) {
		Advance();
		ExprNode* initializer = ParseExpression();
		if (VariableNode* var = dynamic_cast<VariableNode*>(initializer)) {
			if (Accept(NovaTokenType::In)) {
				Advance();
				ExprNode* container = ParseExpression();
				if (Accept(NovaTokenType::OpenBrace)) {
					Advance();
					std::vector<StmtNode*> statements;
					while (!Accept(NovaTokenType::CloseBrace)) {
						statements.push_back(ParseStatement());
					}
					Advance();
					return new ForEachNode(var, container, statements);
				}
				else {
					PushError("Expected {");
				}
			}
			else {
				PushError("Expected 'in'");
			}
		}
		else {
			PushError("Initializer variable is not a variable");
		}
	}
	if (Accept(NovaTokenType::While)) {
		Advance();
		ExprNode* expression = ParseExpression();
		if (Accept(NovaTokenType::OpenBrace)) {
			Advance();
			std::vector<StmtNode*> statements;
			while (!Accept(NovaTokenType::CloseBrace)) {
				statements.push_back(ParseStatement());
			}
			Advance();
			return new WhileNode(expression, statements);
		}
		PushError("Expected {");
	}
	// If nothing else works we try to parse it as a standalone expression
	ExprNode* expr = ParseExpression();
	if (expr) {
		return new ExprAsStmt(expr);
	}

	if (is_const and not handled_const) {
		PushError("Can not evaluate this statement as a constant");
		return nullptr;
	}

	Advance();
	return nullptr;
}

ExprNode* Parser::ParseExpression() {
	ExprNode* expression = ParseAssignment();
	if (expression) {
		return expression;
	}
	else {
		Advance();
		PushError("Unexpected Token Type in expression");
		return nullptr;
	}
}

ExprNode* Parser::ParseTernary() {
	ExprNode* truthy = ParseAssignment();
	if (Accept(NovaTokenType::IfStmt)) {
		Advance();

		ExprNode* condition = ParseExpression();

		if (!Accept(NovaTokenType::ElseStmt)) {
			PushError("Expected 'else' in ternary expression");
			return truthy;
		}
		Advance();

		ExprNode* falsy = ParseExpression();

		return new TernaryNode(condition, truthy, falsy);
	}
	return truthy;
}

ExprNode* Parser::ParseAssignment() {
	ExprNode* expression = ParseBoolean();
	if (Accept(NovaTokenType::Assignment)) {
		Advance();
		ExprNode* right = ParseExpression();
		return new AssignmentNode(expression, right);
	}
	return expression;
}

ExprNode* Parser::ParseBoolean() {
	ExprNode* lhs = ParseEquality();

	while (Accept(NovaTokenType::AndOp) || Accept(NovaTokenType::OrOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseEquality();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseEquality() {
	ExprNode* lhs = ParseComparison();

	while (Accept(NovaTokenType::EqualOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseComparison();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseComparison() {
	ExprNode* lhs = ParseArithmetic();

	while (Accept(NovaTokenType::GreaterOp) || Accept(NovaTokenType::LesserOp) ||
		Accept(NovaTokenType::GtetOp) || Accept(NovaTokenType::LtetOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseArithmetic();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseArithmetic() {
	ExprNode* lhs = ParseTerm();

	while (Accept(NovaTokenType::PlusOp) || Accept(NovaTokenType::MinusOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseTerm();
		lhs = new OpNode(lhs, rhs, op);
	}
	while (Accept(NovaTokenType::CPlusOp) or Accept(NovaTokenType::CMinusOp) or Accept(NovaTokenType::CMultOp) or Accept(NovaTokenType::CDivOp)) {
		std::string op = Current().content;
		Advance();

		ExprNode* rhs = ParseExpression();
		if (rhs) {
			return new CompoundOp(op, lhs, rhs);
		}
		else {
			PushError("Expected an expression");
		}
	}
	return lhs;
}

ExprNode* Parser::ParseTerm() {
	ExprNode* lhs = ParseAccesor();

	while (Accept(NovaTokenType::MultOp) || Accept(NovaTokenType::DivOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseTerm();
		lhs = new OpNode(lhs, rhs, op);
	}

	return lhs;
}

ExprNode* Parser::ParseAccesor() {
	ExprNode* expression = ParseUnary();
	if (Accept(NovaTokenType::Dot)) {
		Advance();
		ExprNode* right = ParsePrimary();
		return new DotAccessNode(expression, right);
	}
	if (Accept(NovaTokenType::OpenBracket)) {
		Advance();
		ExprNode* index = ParseExpression();
		if (!Accept(NovaTokenType::CloseBracket)) {
			PushError("Expected ]");
		}
		Advance();
		return new ArrayAccessNode(expression, index);
	}
	return expression;
}

ExprNode* Parser::ParseUnary() {
	if (Accept(NovaTokenType::Not)) {
		Advance();
		ExprNode* right = ParseUnary();
		return new NotNode(right);
	}

	return ParsePrimary();
}

ExprNode* Parser::ParsePrimary() {
	if (Accept(NovaTokenType::OpenParen)) {
		Advance();
		ExprNode* expr = ParseTernary();
		if (Accept(NovaTokenType::CloseParen)) {
			Advance();
		}
		else {
			PushError("Expected )");
		}
		
		return expr;
	}
	if (Accept(NovaTokenType::IntLit)) {
		int number = std::stoi(Current().content);
		Advance();
		return new IntLiteralNode(number);
	}
	else if (Accept(NovaTokenType::FloatLit)) {
		float number = std::stof(Current().content);
		Advance();
		return new FloatLiteralNode(number);
	}
	else if (Accept(NovaTokenType::StringLit)) {
		std::string content = Current().content;
		Advance();
		return new StringLiteralNode(content);
	}
	else if (Accept(NovaTokenType::BoolLit)) {
		bool value = Current().content == "true" ? true : false;
		Advance();
		return new BoolLiteralNode(value);
	}
	else if (Accept(NovaTokenType::Identifier)) {
		std::string id = Current().content;
		Advance();
		// Function Call
		if (Accept(NovaTokenType::OpenParen)) {
			std::vector<ExprNode*> args;
			Advance();
			while (!Accept(NovaTokenType::CloseParen)) {
				ExprNode* arg = ParseExpression();
				args.push_back(arg);
			}
			Advance();
			return new FuncCallNode(id, args);
		}
		return new VariableNode(id);
	}
	else if (Accept(NovaTokenType::OpenBracket)) {
		std::vector<ExprNode*> values;
		Advance();
		while(!Accept(NovaTokenType::CloseBracket)) {
			values.push_back(ParseExpression());
		}
		Advance();
		return new ArrayLiteralNode(values);
	}
	else if (Accept(NovaTokenType::Null)) {
		Advance();
		return new NullLiteralNode();
	}
	return nullptr;
}

void Parser::PushError(const std::string& message) {
	force_stop = true;
	Callbacker::PushError(("[Parser] " + message + " at " + std::to_string(Current().line) + ", " + std::to_string(Current().column) + ", NovaTokenType is " + std::to_string(int(Current().type))).c_str(), 2);
}

void Parser::PushWarning(const std::string& message) {
	Callbacker::PushError(("[Parser] " + message + " at " + std::to_string(Current().line) + ", " + std::to_string(Current().column) + ", NovaTokenType is " + std::to_string(int(Current().type))).c_str(), 1);
}