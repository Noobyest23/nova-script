#ifndef NOVASCRIPT_AST_H
#define NOVASCRIPT_AST_H

#include <string>

struct StmtNode;
struct ExprNode;

struct ASTNode {
	bool constant = false;
	virtual std::string Print() const {
		return "Not Implemented";
	};

	virtual void Delete() {
		delete this;
	};
};


#endif