#ifndef NOVASCRIPT_PROGRAM_NODE_H
#define NOVASCRIPT_PROGRAM_NODE_H

#include "ASTNode.h"
#include <vector>
#include "StmtNode.h"

struct ProgramNode : public ASTNode {
	ProgramNode(std::vector<StmtNode*>& statements) : Statements(statements) {};

	std::vector<StmtNode*> Statements;

	bool valid_program = false;

	

	std::string Print() const override {
		std::string result;
		for (StmtNode* stmt : Statements) {
			if (stmt) {
				result += stmt->Print() + "\n";
			}
		}
		return result;
	}

	void Delete() override {
		for (StmtNode* stmt : Statements) {
			if (stmt) {
				stmt->Delete();
			}
		}

		delete this;
	}

};






#endif