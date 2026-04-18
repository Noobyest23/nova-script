#ifndef NOVASCRIPT_AST_H
#define NOVASCRIPT_AST_H

#include <string>

struct StmtNode;
struct ExprNode;

struct ASTNode {
	virtual std::string Print() const {
		return "Not Implemented";
	};

	int line, column;
	bool constant = false;

	virtual void Delete() = 0;
};


#endif