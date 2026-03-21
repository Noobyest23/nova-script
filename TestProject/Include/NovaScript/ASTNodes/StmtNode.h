#ifndef NOVASCRIPT_STMT_NODE_H
#define NOVASCRIPT_STMT_NODE_H

#include "ASTNode.h"
#include "ExprNode.h"

struct StmtNode : public ASTNode {

};

struct VarDeclNode : public StmtNode {
	VarDeclNode(const std::string& id, ExprNode* rhs) : identifier(id), right(rhs) {};

	std::string identifier;
	ExprNode* right;
	
	std::string Print() const override {
		if (right) {
			return "var " + identifier + " = " + right->Print();
		}
		else {
			return "var " + identifier;
		}
	}

	void Delete() override {
		if (right) {
			right->Delete();
		}
		delete this;
	}

};

struct FuncDeclNode : public StmtNode {
	FuncDeclNode(const std::string& func_id, const std::vector<std::string>& args, std::vector<StmtNode*> body) : func_id(func_id), args(args), body(body) {};

	std::string func_id;
	std::vector<std::string> args;
	std::vector<StmtNode*> body;

	std::string Print() const override {
		std::string output = func_id + "(";
		for (const std::string& arg : args) {
			output += arg + (arg == args[args.size() - 1] ? "" : ", ");
		}
		output += ") {\n";
		for (StmtNode* statement : body) {
			if (statement) {
				output += statement->Print() + "\n";
			}
		}
		output += "}";
		return output;
	}

	void Delete() override {
		for (StmtNode* statement : body) {
			if (statement) {
				statement->Delete();
			}
		}
		delete this;
	}
};

struct IfStmtNode : public StmtNode {
	IfStmtNode(ExprNode* expr, std::vector<StmtNode*> body, std::vector<StmtNode*> else_body) : expression(expr), body(body), else_body(else_body) {};

	ExprNode* expression;
	std::vector<StmtNode*> body;
	std::vector<StmtNode*> else_body;

	std::string Print() const override {
		std::string output = "if (" + expression->Print() + ") {\n";
		for (StmtNode* stmt : body) {
			if (stmt) {
				output += stmt->Print() + "\n";
			}
		}
		output += "}";
		if (else_body.size() > 0) {
			output += "\nelse {\n";
			for (StmtNode* stmt : else_body) {
				if (stmt) {
					output += stmt->Print() + "\n";
				}
			}
			output += "}";
		}
		return output;
	}

	void Delete() override {
		for (StmtNode* stmt : body) {
			if (stmt) {
				stmt->Delete();
			}
		}
		for (StmtNode* stmt : else_body) {
			if (stmt) {
				stmt->Delete();
			}
		}
		if (expression) {
			expression->Delete();
		}
		delete this;
	}

};

struct TypeDeclNode : public StmtNode {
	TypeDeclNode(const std::string& type_name, const std::string& mirror_name, const std::vector<StmtNode*>& definition) : type_name(type_name), mirror_name(mirror_name), definition(definition) {};

	std::string type_name;
	std::string mirror_name;
	std::vector<StmtNode*> definition;

	std::string Print() const override {
		std::string output = type_name + (mirror_name != "" ? " : " + mirror_name : "") + " {\n";
		for (StmtNode* stmt : definition) {
			if (stmt) {
				output += stmt->Print() + "\n";
			}
		}
		output += "}";
		return output;
	}

	void Delete() override {
		for (StmtNode* stmt : definition) {
			if (stmt) {
				stmt->Delete();
			}
		}
		delete this;
	}

};

struct IncludeNode : public StmtNode {
	IncludeNode(ExprNode* file_path) : file_path(file_path) {};
	IncludeNode(ExprNode* file_path, ExprNode* as) : file_path(file_path), as(as) {};
	ExprNode* file_path;
	ExprNode* as = nullptr;

	std::string Print() const override {
		if (file_path) {
			if (as) {
				return "include " + file_path->Print() + " as " + as->Print();
			}
			return "include " + file_path->Print();
		}
		else {
			return "include ERROR_EXPR_NULL";
		}
	}

	void Delete() override {
		if (file_path) {
			file_path->Delete();
		}
		if (as) {
			as->Delete();
		}
		delete this;
	}

};

struct ReturnStmtNode : public StmtNode {
	ReturnStmtNode(ExprNode* return_value) : return_value(return_value) {};
	ExprNode* return_value;

	std::string Print() const override {
		if (return_value) {
			return "return " + return_value->Print();
		}
		else {
			return "return " + std::string("ERROR_EXPR_NULl");
		}
	}

	void Delete() override {
		if (return_value) {
			return_value->Delete();
		}
		delete this;
	}

};

struct BreakPointNode : public StmtNode {
	BreakPointNode(StmtNode* stmt) : stmt(stmt) {};

	StmtNode* stmt;

	std::string Print() const override {
		if (stmt) {
			return "| " + stmt->Print();
		}
		else {
			return "| " + std::string("ERROR_STMT_NULL");
		}
	}

	void Delete() override {
		if (stmt) {
			stmt->Delete();
		}
		delete this;
	}
};

struct ASTPrintNode : public StmtNode {

	std::string Print() const override {
		return "PRINT TREE";
	}

};

struct ForEachNode : public StmtNode {
	ForEachNode(ExprNode* variable, ExprNode* container, std::vector<StmtNode*>& body) : variable(variable), container(container), body(body) {};

	ExprNode* variable;
	ExprNode* container;
	std::vector<StmtNode*> body;

	std::string Print() const override {
		std::string con = (container ? container->Print() : "ERROR_EXPR_NULL");
		std::string iterator_name = (variable ? variable->Print() : "ERROR_EXPR_NULL");
		std::string out = "for (" + iterator_name + " in " + con + ") {\n";
		for (StmtNode* node : body) {
			if (node) {
				out += node->Print() + "\n";
			}
		}
		out += "}";
		return out;
	}

	void Delete() override {
		if (container) {
			container->Delete();
		}
		if (variable) {
			variable->Delete();
		}
		for (StmtNode* node : body) {
			if (node) {
				node->Delete();
			}
		}
		delete this;
	}

};

struct WhileNode : public StmtNode {
	WhileNode(ExprNode* expression, std::vector<StmtNode*> body) : expression(expression), body(body) {};
	ExprNode* expression;
	std::vector<StmtNode*> body;

	std::string Print() const override {
		std::string expr = (expression ? expression->Print() : "ERROR_EXPR_NULL");
		std::string out = "while (" + expr + ") {\n";
		for (StmtNode* node : body) {
			if (node) {
				out += node->Print() + "\n";
			}
		}
		out += "}";
		return out;
	}

	void Delete() override {
		if (expression) {
			expression->Delete();
		}
		for (StmtNode* node : body) {
			if (node) {
				node->Delete();
			}
		}
	}


};

struct ExprAsStmt : public StmtNode {
	ExprAsStmt(ExprNode* expr) : expr(expr) {};

	ExprNode* expr;

	std::string Print() const override {
		return expr->Print() + " as Stmt";
	}

	void Delete() override {
		expr->Delete();
		delete this;
	}

};



#endif