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
	program->line = 0;
	program->column = 0;
	return program;
}

StmtNode* Parser::ParseStatement() {
	// Variable Declaration Node
	bool is_const = false;
	bool handled_const = false;
	if (Accept(NovaTokenType::Eof)) {
		return nullptr;
	}
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
			auto* node = new VarDeclNode(var_name, expression);
			node->line = Current().line;
			node->column = Current().column;
			node->constant = is_const;
			return node;
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
					if (Accept(NovaTokenType::Comma)) {
						Advance();
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
				auto* node = new FuncDeclNode(func_name, args, body);
				node->line = Current().line;
				node->column = Current().column;
				return node;
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
			auto* node = new IfStmtNode(expression, body, else_body);
			node->line = Current().line;
			node->column = Current().column;
			return node;
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
				auto* node = new TypeDeclNode(type_id, mirror_id, scope);
				node->line = Current().line;
				node->column = Current().column;
				return node;
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
				auto* node = new IncludeNode(expr, as);
				node->line = Current().line;
				node->column = Current().column;
				return node;
			}
			auto* node = new IncludeNode(expr);
			node->line = Current().line;
			node->column = Current().column;
			return node;
		}
		else {
			PushError("Expected an expression");
		}
	}
	if (Accept(NovaTokenType::Return)) {
		Advance();
		ExprNode* return_value = ParseExpression();
		if (return_value) {
			auto* node = new ReturnStmtNode(return_value);
			node->line = Current().line;
			node->column = Current().column;
			return node;
		}
		else {
			PushError("Expected an expression");
		}
	}
	if (Accept(NovaTokenType::BreakPoint)) {
		Advance();
		StmtNode* stmt = nullptr;
		if (t_index != tokens.size()) {
			stmt = ParseStatement();
		}
		auto* node = new BreakPointNode(stmt);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	if (Accept(NovaTokenType::ASTPrint)) {
		Advance();
		return new ASTPrintNode;
	}
	if (Accept(NovaTokenType::For)) {
		Advance();
		// Optional parenthesis
		if (Accept(NovaTokenType::OpenParen)) {
			Advance();
		}
		ExprNode* initializer = ParseExpression();
		if (VariableNode* var = dynamic_cast<VariableNode*>(initializer)) {
			if (Accept(NovaTokenType::In)) {
				Advance();
				ExprNode* container = ParseExpression();
				if (Accept(NovaTokenType::CloseParen)) {
					Advance();
				}
				if (Accept(NovaTokenType::OpenBrace)) {
					Advance();
					std::vector<StmtNode*> statements;
					while (!Accept(NovaTokenType::CloseBrace)) {
						statements.push_back(ParseStatement());
					}
					Advance();
					auto* node = new ForEachNode(var, container, statements);
					node->line = Current().line;
					node->column = Current().column;
					return node;
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
			auto* node = new WhileNode(expression, statements);
			node->line = Current().line;
			node->column = Current().column;
			return node;
		}
		PushError("Expected {");
	}
	// If nothing else works we try to parse it as a standalone expression
	ExprNode* expr = ParseExpression();
	if (expr) {
		auto* node = new ExprAsStmt(expr);
		node->line = Current().line;
		node->column = Current().column;
		return node;
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

		auto* node = new TernaryNode(condition, truthy, falsy);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	return truthy;
}

ExprNode* Parser::ParseAssignment() {
	ExprNode* expression = ParseBoolean();
	if (Accept(NovaTokenType::Assignment)) {
		Advance();
		ExprNode* right = ParseExpression();
		auto* node = new AssignmentNode(expression, right);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	return expression;
}

ExprNode* Parser::ParseBoolean() {
	ExprNode* lhs = ParseEquality();

	while (Accept(NovaTokenType::AndOp) || Accept(NovaTokenType::OrOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseEquality();
		auto* node = new OpNode(lhs, rhs, op);
		node->line = Current().line;
		node->column = Current().column;
		lhs = node;
	}

	return lhs;
}

ExprNode* Parser::ParseEquality() {
	ExprNode* lhs = ParseComparison();

	while (Accept(NovaTokenType::EqualOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseComparison();
		auto* node = new OpNode(lhs, rhs, op);
		node->line = Current().line;
		node->column = Current().column;
		lhs = node;
	}

	while (Accept(NovaTokenType::Is)) {
		Advance();
		ExprNode* rhs = ParseComparison();
		auto* node = new IsNode(lhs, rhs);
		node->line = Current().line;
		node->column = Current().column;
		lhs = node;
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
		auto* node = new OpNode(lhs, rhs, op);
		node->line = Current().line;
		node->column = Current().column;
		lhs = node;
	}

	return lhs;
}

ExprNode* Parser::ParseArithmetic() {
	ExprNode* lhs = ParseTerm();

	while (Accept(NovaTokenType::PlusOp) || Accept(NovaTokenType::MinusOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseTerm();
		auto* node = new OpNode(lhs, rhs, op);
		node->line = Current().line;
		node->column = Current().column;
		lhs = node;
	}
	while (Accept(NovaTokenType::CPlusOp) or Accept(NovaTokenType::CMinusOp) or Accept(NovaTokenType::CMultOp) or Accept(NovaTokenType::CDivOp) or Accept(NovaTokenType::CModOp)) {
		std::string op = Current().content;
		Advance();

		ExprNode* rhs = ParseExpression();
		if (rhs) {
			auto* node = new CompoundOp(op, lhs, rhs);
			node->line = Current().line;
			node->column = Current().column;
			lhs = node;
		}
		else {
			PushError("Expected an expression");
		}
	}
	return lhs;
}

ExprNode* Parser::ParseTerm() {
	ExprNode* lhs = ParseAccesor();

	while (Accept(NovaTokenType::MultOp) || Accept(NovaTokenType::DivOp) || Accept(NovaTokenType::ModOp)) {
		std::string op = Current().content;
		Advance();
		ExprNode* rhs = ParseTerm();
		auto* node = new OpNode(lhs, rhs, op);
		node->line = Current().line;
		node->column = Current().column;
		lhs = node;
	}

	return lhs;
}

ExprNode* Parser::ParseAccesor() {
	ExprNode* expression = ParseUnary();
	if (Accept(NovaTokenType::Dot)) {
		Advance();
		ExprNode* right = ParsePrimary();
		auto* node = new DotAccessNode(expression, right);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	if (Accept(NovaTokenType::OpenBracket)) {
		Advance();
		ExprNode* index = ParseExpression();
		if (!Accept(NovaTokenType::CloseBracket)) {
			PushError("Expected ]");
		}
		Advance();
		auto* node = new ArrayAccessNode(expression, index);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	return expression;
}

ExprNode* Parser::ParseUnary() {
	if (Accept(NovaTokenType::Not)) {
		Advance();
		ExprNode* right = ParseUnary();
		auto* node = new NotNode(right);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	
	if (Accept(NovaTokenType::MinusOp)) {
		Advance();
		ExprNode* right = ParseUnary();
		auto* node = new NotNode(right);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	return ParsePrimary();
}

ExprNode* Parser::ParsePrimary() {
	bool as_ptr = false;
	if (Accept(NovaTokenType::MultOp)) {
		as_ptr = true;
		Advance();
	}
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
		auto* node = new IntLiteralNode(number);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	else if (Accept(NovaTokenType::FloatLit)) {
		float number = std::stof(Current().content);
		Advance();
		auto* node = new FloatLiteralNode(number);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	else if (Accept(NovaTokenType::StringLit)) {
		std::string content = Current().content;
		Advance();
		auto* node = new StringLiteralNode(content);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	else if (Accept(NovaTokenType::BoolLit)) {
		bool value = Current().content == "true" ? true : false;
		Advance();
		auto* node = new BoolLiteralNode(value);
		node->line = Current().line;
		node->column = Current().column;
		return node;
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
				if (Accept(NovaTokenType::Comma)) {
					Advance();
				}
				args.push_back(arg);
			}
			Advance();
			auto* node = new FuncCallNode(id, args);
			node->line = Current().line;
			node->column = Current().column;
			return node;
		}
		auto* node = new VariableNode(id, as_ptr);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	else if (Accept(NovaTokenType::This)) {
		Advance();
		auto* node = new VariableNode("_NOVA_THIS", as_ptr);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	else if (Accept(NovaTokenType::OpenBracket)) {
		std::vector<ExprNode*> values;
		Advance();
		while(!Accept(NovaTokenType::CloseBracket)) {
			values.push_back(ParseExpression());
			if (Accept(NovaTokenType::Comma)) {
				Advance();
			}
		}
		Advance();
		auto* node = new ArrayLiteralNode(values);
		node->line = Current().line;
		node->column = Current().column;
		return node;
	}
	else if (Accept(NovaTokenType::Null)) {
		Advance();
		auto* node = new NullLiteralNode();
		node->line = Current().line;
		node->column = Current().column;
		return node;
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