#include "Optimizer.h"
#include "../ASTNodes/ProgramNode.h"
#include "../Interpretor/Interpretor.h"
#include "../Interpretor/Value.h"
#include "../Interpretor/Scope.h"

ProgramNode* Optimizer::Optimize() {
	const_evaluator = new Interpretor(nullptr);
	std::vector<StmtNode*> new_statements;
	for (StmtNode* statement : old_program->Statements) {
		StmtNode* n_stmt = OptimizeStatement(statement);
		if (n_stmt) {
			new_statements.push_back(n_stmt);
		}
		if (emplace_block_flag) {
			for (StmtNode* bstmt : block_stack) {
				new_statements.push_back(bstmt);
			}
			block_stack.clear();
			emplace_block_flag = false;
		}
	}
	delete old_program;
	ProgramNode* new_program = new ProgramNode(new_statements);
	new_program->valid_program = true;

	delete const_evaluator;
	return new_program;
}

StmtNode* Optimizer::OptimizeStatement(StmtNode* statement) {
	if (!statement) {
		return nullptr;
	}
	if (VarDeclNode* var_decl = dynamic_cast<VarDeclNode*>(statement)) {
		if (var_decl->constant) {
			
			Value right = const_evaluator->EvaluateExpression(var_decl->right);
			
			const_evaluator->Set(var_decl->identifier, right);
			var_decl->Delete();
			return nullptr;
		}
		else {
			var_decl->right = OptimizeExpression(var_decl->right);
			return var_decl;
		}
	}
	else if (ExprAsStmt* expr = dynamic_cast<ExprAsStmt*>(statement)) {
		expr->expr = OptimizeExpression(expr->expr);
		return expr;
	}
	else if (BreakPointNode* stmt = dynamic_cast<BreakPointNode*>(statement)) {
		stmt->stmt = OptimizeStatement(stmt->stmt);
		if (!stmt->stmt) {
			stmt->Delete();
			return nullptr;
		}
		return stmt;
	}
	else if (IfStmtNode* ifstmt = dynamic_cast<IfStmtNode*>(statement)) {
		const_evaluator->PushScope();
		std::vector<StmtNode*> new_body;
		for (StmtNode* stmt : ifstmt->body) {
			StmtNode* new_stmt = OptimizeStatement(stmt);
			if (new_stmt) {
				new_body.push_back(new_stmt);
			}
		}
		ifstmt->body = new_body;
		new_body.clear();
		for (StmtNode* stmt : ifstmt->else_body) {
			StmtNode* new_stmt = OptimizeStatement(stmt);
			if (new_stmt) {
				new_body.push_back(new_stmt);
			}
		}
		ifstmt->else_body = new_body;
		ExprNode* condition = OptimizeExpression(ifstmt->expression);
		if (BoolLiteralNode* lit = dynamic_cast<BoolLiteralNode*>(condition)) {
			std::vector<StmtNode*> target_body = lit->value ? ifstmt->body : ifstmt->else_body;
			block_stack = target_body;
			emplace_block_flag = true;
			const_evaluator->PopScope();
			return nullptr;
		}

		const_evaluator->PopScope();
		return ifstmt;
	}
	else if (FuncDeclNode* function = dynamic_cast<FuncDeclNode*>(statement)) {
		const_evaluator->PushScope();
		std::vector<StmtNode*> new_body;
		for (StmtNode* stmt : function->body) {
			StmtNode* new_stmt = OptimizeStatement(stmt);
			if (new_stmt) {
				new_body.push_back(new_stmt);
			}
		}
		function->body = new_body;
		const_evaluator->PopScope();
		return function;
	}
	else {
		return statement;
	}
}

ExprNode* Optimizer::OptimizeExpression(ExprNode* expression) {
	if (!expression) {
		return nullptr;
	}
	if (VariableNode* var = dynamic_cast<VariableNode*>(expression)) {
		if (Value* value = const_evaluator->Get(var->identifier)) {
			var->Delete();
			return CreateLiteralFromValue(value);
		}
		else {
			return expression;
		}
	}
	else if (AssignmentNode* ass = dynamic_cast<AssignmentNode*>(expression)) {
		ass->right = OptimizeExpression(ass->right);
		return ass;
	}
	else if (OpNode* op = dynamic_cast<OpNode*>(expression)) {
		op->left = OptimizeExpression(op->left);
		op->right = OptimizeExpression(op->right);
		bool left_is_const = op->left->constant;
		bool right_is_const = op->right->constant;
		if (VariableNode* var = dynamic_cast<VariableNode*>(op->left)) {
			left_is_const = const_evaluator->Get(var->identifier);
		}
		if (VariableNode* var = dynamic_cast<VariableNode*>(op->right)) {
			right_is_const = const_evaluator->Get(var->identifier);
		}
		if (left_is_const and right_is_const) {
			Value value = const_evaluator->EvaluateExpression(expression);
			op->Delete();
			return CreateLiteralFromValue(&value);
		}
		else {
			return op;
		}
	}
	else if (CompoundOp* op = dynamic_cast<CompoundOp*>(expression)) {
		op->rhs = OptimizeExpression(op->rhs);
		return op;
	}
	else {
		return expression;
	}
}

ExprNode* Optimizer::CreateLiteralFromValue(Value* value) {
	if (!value) {
		return nullptr;
	}
	if (value->IsInt()) {
		int& val = value->GetInt();
		return new IntLiteralNode(val);
	}
	if (value->IsFloat()) {
		float& val = value->GetFloat();
		return new FloatLiteralNode(val);
	}
	if (value->IsBool()) {
		bool& val = value->GetBool();
		return new BoolLiteralNode(val);
	}
	if (value->IsArray()) {
		std::vector<Value>& val = value->GetArray();
		std::vector<ExprNode*> lit_values;
		for (Value v : val) {
			lit_values.push_back(CreateLiteralFromValue(&v));
		}
		return new ArrayLiteralNode(lit_values);
	}
	if (value->IsVec2()) {
		glm::vec2& val = value->GetVec2();
		ExprNode* v1 = new FloatLiteralNode(val.x);
		ExprNode* v2 = new FloatLiteralNode(val.y);
		std::vector<ExprNode*> values = { v1, v2 };
		return new Vector2LiteralNode(values);
	}if (value->IsVec3()) {
		glm::vec3& val = value->GetVec3();
		ExprNode* v1 = new FloatLiteralNode(val.x);
		ExprNode* v2 = new FloatLiteralNode(val.y);
		ExprNode* v3 = new FloatLiteralNode(val.z);
		std::vector<ExprNode*> values = { v1, v2, v3 };
		return new Vector3LiteralNode(values);
	}if (value->IsVec4()) {
		glm::vec4& val = value->GetVec4();
		ExprNode* v1 = new FloatLiteralNode(val.x);
		ExprNode* v2 = new FloatLiteralNode(val.y);
		ExprNode* v3 = new FloatLiteralNode(val.z);
		ExprNode* v4 = new FloatLiteralNode(val.w);
		std::vector<ExprNode*> values = { v1, v2, v3, v4 };
		return new Vector4LiteralNode(values);
	}
	if (value->IsString()) {
		std::string& val = value->GetString();
		return new StringLiteralNode(val);
	}
	
	throw std::exception(("Value has type that disqualifys it from being constant. type is " + value->Type()).c_str());
}
