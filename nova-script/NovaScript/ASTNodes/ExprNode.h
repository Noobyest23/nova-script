#ifndef NOVASCRIPT_EXPR_NODE_H
#define NOVASCRIPT_EXPR_NODE_H

#include "ASTNode.h"
#include <vector>

struct ExprNode : public ASTNode {
	
};

struct VariableNode : public ExprNode {
	VariableNode(const std::string& id, bool as_ptr = false) : identifier(id), as_ptr(as_ptr) { };
	std::string identifier;
	bool as_ptr = false;
	std::string Print() const override {
		return identifier;
	}

	void Delete() override{
		identifier.clear();
		delete this;
	}
};

struct OpNode : public ExprNode {
	OpNode(ExprNode* lhs, ExprNode* rhs, const std::string& op) : left(lhs), right(rhs), op(op) {};

	ExprNode* left;
	ExprNode* right;
	std::string op;

	std::string Print() const override {
		return left->Print() + " " + op + " " + right->Print();
	}

	void Delete() override {
		left->Delete();
		right->Delete();
		delete this;
	}

};

struct FuncCallNode : public ExprNode {
	FuncCallNode(const std::string& func_id, const std::vector<ExprNode*>& args) : func_id(func_id), args(args) {};

	std::string func_id;
	std::vector<ExprNode*> args;

	std::string Print() const override {
		std::string output = func_id + "(";
		for (ExprNode* arg : args) {
			if (arg) {
				output += arg->Print() + (arg == args[args.size() - 1] ? "" : ", ");
			}
		}
		output += ")";
		return output;
	}

	void Delete() override {
		for (ExprNode* arg : args) {
			if (arg) {
				arg->Delete();
			}
		}
		delete this;
	}

};

struct AssignmentNode : public ExprNode {
	AssignmentNode(ExprNode* left, ExprNode* right) : left(left), right(right) {};

	ExprNode* left;
	ExprNode* right;

	std::string Print() const override {
		return left->Print() + " = " + right->Print();
	}

	void Delete() override {
		left->Delete();
		right->Delete();
		delete this;
	}

};

struct CompoundOp : public ExprNode {
	CompoundOp(const std::string& op, ExprNode* lhs, ExprNode* rhs) : op(op), lhs(lhs), rhs(rhs) {};

	std::string op;
	ExprNode* lhs;
	ExprNode* rhs;

	std::string Print() const override {
		return lhs->Print() + " " + op + " " + rhs->Print();
	}

	void Delete() override {
		lhs->Delete();
		rhs->Delete();
		delete this;
	}

};

struct TernaryNode : public ExprNode {
	TernaryNode(ExprNode* expression, ExprNode* truthy_value, ExprNode* falsey_value) : expression(expression), truthy_value(truthy_value), falsey_value(falsey_value) {};

	ExprNode* expression;
	ExprNode* truthy_value;
	ExprNode* falsey_value;

	std::string Print() const override {
		std::string output;
		if (truthy_value) {
			output += truthy_value->Print();
		}
		else {
			output += "ERROR_EXPR_NULL";
		}
		output += " if ";
		if (expression) {
			output += expression->Print();
		}
		else {
			output += "ERROR_EXPR_NULL";
		}
		output += " else ";
		if (falsey_value) {
			output += falsey_value->Print();
		}
		else {
			output += "ERROR_EXPR_NULL";
		}
		return output;
	}

	void Delete() override {
		if (expression) {
			delete expression;
		}
		if (truthy_value) {
			delete truthy_value;
		}
		if (falsey_value) {
			delete falsey_value;
		}
		delete this;
	}

};

struct IntLiteralNode : public ExprNode {
	IntLiteralNode(int number) : number(number) { constant = true; };

	int number;

	std::string Print() const override {
		return std::to_string(number);
	}

	void Delete() override {
		delete this;
	}

};

struct FloatLiteralNode : public ExprNode {
	FloatLiteralNode(float number) : number(number) { constant = true; };

	float number;

	std::string Print() const override {
		return std::to_string(number);
	}

	void Delete() override {
		delete this;
	}

};

struct StringLiteralNode : public ExprNode {
	StringLiteralNode(const std::string& string) : string(string) { constant = true; };

	std::string string;

	std::string Print() const override {
		return '"' + string + '"';
	}

	void Delete() override {
		string.clear();
		delete this;
	}

};

struct BoolLiteralNode : public ExprNode {
	BoolLiteralNode(bool value) : value(value) { constant = true; };

	bool value;

	std::string Print() const override {
		return value ? "true" : "false";
	}

	void Delete() override {
		delete this;
	}

};

struct ArrayLiteralNode : public ExprNode {
	ArrayLiteralNode(std::vector<ExprNode*>& values) : values(values) { constant = true; }

	std::vector<ExprNode*> values;

	std::string Print() const override {
		std::string content = "[";
		for (ExprNode* value : values) {
			if (value) {
				content += value->Print() + (value == values.back() ? "]" : ", ");
			}
		}
		return content;
	}

	void Delete() override {
		for (ExprNode* value : values) {
			if (value) {
				value->Delete();
			}
		}
		delete this;
	}

};

// Var -> DotAccess -> DotAccess -> CPPVar
// engine.window.size
struct DotAccessNode : public ExprNode {
	DotAccessNode(ExprNode* left, ExprNode* right) : left(left), right(right) {};

	ExprNode* left;
	ExprNode* right;

	std::string Print() const override {
		std::string content;
		if (left) {
			content += left->Print() + ".";
		}
		else {
			content += "ERROR_EXPR_NULL" + std::string(".");
		}
		if (right) {
			content += right->Print();
		}
		else {
			content += "ERROR_EXPR_NULL";
		}
		return content;
	}

	void Delete() override {
		if (left) {
			left->Delete();
		}
		if (right) {
			right->Delete();
		}
		delete this;
	}

};

struct ArrayAccessNode : public ExprNode {
	ArrayAccessNode(ExprNode* arr, ExprNode* index) : arr(arr), index(index) {};

	ExprNode* arr;
	ExprNode* index;

	std::string Print() const override {
		std::string output;
		if (arr) {
			output += arr->Print();
		}
		else {
			output += "ERROR_EXPR_NULL";
		}
		if (index) {
			output += "[" + index->Print() + "]";
		}
		else {
			output += "[ERROR_EXPR_NULL]";
		}
		return output;
	}

	void Delete() override {
		if (arr) {
			arr->Delete();
		}
		if (index) {
			index->Delete();
		}
		delete this;
	}

};

struct OptimizedExpr : public ExprNode {

	std::string Print() const override {
		return "This expression was optimized away, and is unavailable";
	}

	void Delete() override {
		delete this;
	}

};

struct NullLiteralNode : public ExprNode {

	std::string Print() const override {
		return "null";
	}

	void Delete() override {
		delete this;
	}

};

struct NotNode : public ExprNode {
	NotNode(ExprNode* expression) : expression(expression) {};
	ExprNode* expression;

	std::string Print() const override {
		if (expression) {
			return "not " + expression->Print();
		}
		else {
			return "not ERROR_EXPR_NULL";
		}
	}

	void Delete() override {
		if (expression) {
			expression->Delete();
		}
		delete this;
	}

};

struct IsNode : public ExprNode {
	IsNode(ExprNode* lhs, ExprNode* rhs) : lhs(lhs), rhs(rhs) {};
	ExprNode* lhs;
	ExprNode* rhs;

	std::string Print() const override {
		std::string output;
		if (lhs) {
			output = lhs->Print();
		}
		else {
			output = "ERROR_EXPR_NULL";
		}
		output += " is ";
		if (rhs) {
			output += rhs->Print();
		}
		else {
			output += "ERROR_EXPR_NULL";
		}
		return output;
	}

	void Delete() override {
		if (lhs) {
			lhs->Delete();
		}
		if (rhs) {
			rhs->Delete();
		}

		delete this;
	}

};

#endif