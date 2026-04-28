#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Interpretor/Scope.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Value/BoolValue.h"
#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FloatValue.h"
#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/ArrayValue.h"
#include "../NovaScript/Value/NovaObject.h"
#include "../NovaScript/Library/nova_math.h"

#undef ee
#define ee_decl(type) std::shared_ptr<NovaValue> Interpretor::EvaluateExpression(type)
#define ee(type) if (type n = dynamic_cast<type>(node)) {return EvaluateExpression(n);}

#define STR(type) #type

ee_decl(ExprNode* node) {
	if (!node) {
		return null;
	}
	ee(VariableNode*)
	ee(OpNode*)
	ee(FuncCallNode*)
	ee(TernaryNode*)
	ee(IntLiteralNode*)
	ee(FloatLiteralNode*)
	ee(BoolLiteralNode*)
	ee(StringLiteralNode*)
	ee(ArrayLiteralNode*)
	ee(DotAccessNode*)
	ee(ArrayAccessNode*)
	ee(AssignmentNode*)
	ee(CompoundOp*)
	ee(NullLiteralNode*)
	ee(NotNode*)
	ee(IsNode*)
	PushError("Unrecognized Expression", node);
	return null;
}

ee_decl(VariableNode* node) {
	if (scope->Has(node->identifier)) {
		std::shared_ptr<NovaValue> val = scope->Get(node->identifier);
		return val;
	}
	else {
		PushError("Variable, " + node->identifier + " does not exist in the scope", node);
		return null;
	}
}

ee_decl(AssignmentNode* node) {
	std::shared_ptr<NovaValue> lhs = EvaluateExpression(node->left);

	std::shared_ptr<NovaValue> rhs = nullptr;
	if (VariableNode* v = dynamic_cast<VariableNode*>(node->right)) {
		if (v->as_ptr) {
			rhs = scope->Get(v->identifier);
		}
		else {
			rhs = scope->Get(v->identifier)->Copy();
		}
	}
	else {
		rhs = EvaluateExpression(node->right);
	}

	if (VariableNode* var = dynamic_cast<VariableNode*>(node->left)) {
		if (!lhs->Assign(rhs)) {
			scope->Set(var->identifier, rhs);
		}
		return lhs;
	}
	else if (DotAccessNode* dot = dynamic_cast<DotAccessNode*>(node->left)) {
		if (VariableNode* var = dynamic_cast<VariableNode*>(dot->right)) {
			if (!lhs->Assign(rhs)) {
				lhs = EvaluateExpression(dot->left); // gives us the var in var.property
				lhs->accessables->insert_or_assign(var->identifier, rhs);
				return rhs;
			}
			return lhs;
		}
		else {
			PushError("Unrecognized node in dot access assignment", node);
			return null;
		}
	}
	else if (ArrayAccessNode* arr = dynamic_cast<ArrayAccessNode*>(node->left)) {
		if (lhs->Type() == "Array") {
			if (!lhs->Assign(rhs)) {

				lhs = EvaluateExpression(arr->arr); // gives us the array itself, where lhs was just what we got from array at the index
				NovaArray* narr = static_cast<NovaArray*>(lhs.get());

				std::shared_ptr<NovaValue> index = EvaluateExpression(arr->index);
				if (!index or index->Type() != "Int") { PushError("Index in array assignment is not an int", node); return null; }
				NovaInt* int_index = static_cast<NovaInt*>(index.get());
				if (int_index->CNum() > narr->CArr().size()) { PushError("Out of bounds array assignment", node); return null; }


				(*narr->Arr())[int_index->CNum()] = rhs;
				return narr->CArr()[int_index->CNum()];
				
			}
			return lhs;
		}
	}
	
}

ee_decl(CompoundOp* node) {
	std::shared_ptr<NovaValue> lhs = EvaluateExpression(node->lhs);
	std::shared_ptr<NovaValue> rhs = EvaluateExpression(node->rhs);

	NovaValue::NovaOperator op = NovaValue::CompoundPlus;

	if (node->op == "/=") {
		std::shared_ptr<NovaValue> rounded = nova_float_to_int(rhs);
		if (rounded->Type() == "Int") {
			NovaInt* nova_i = static_cast<NovaInt*>(rounded.get());
			if (nova_i->CNum() == 0) {
				PushError("Cannot divide by 0", node);
				return null;
			}
		}
	}

	if (node->op == "-=") {
		op = NovaValue::CompoundMinus;
	}
	else if (node->op == "*=") {
		op = NovaValue::CompoundMultiply;
	}
	else if (node->op == "/=") {
		op = NovaValue::CompoundDivide;
	}
	else if (node->op == "%=") {
		op = NovaValue::CompoundMod;
	}

	bool result = lhs->PerformCompoundOp(rhs, op);
	if (result) {
		return lhs;
	}
	else {
		PushError("Failed to perform compound operation of (" + lhs->Type() + NovaValue::OpToString(op) + rhs->Type() + ")", node);
		return null;
	}
}

ee_decl(OpNode* node) {
	std::shared_ptr<NovaValue> lhs = EvaluateExpression(node->left);
	std::shared_ptr<NovaValue> rhs = EvaluateExpression(node->right);

	NovaValue::NovaOperator op = NovaValue::Plus;
	
	if (node->op == "/") {
		std::shared_ptr<NovaValue> rounded = nova_float_to_int(rhs);
		if (rounded->Type() == "Int") {
			NovaInt* nova_i = static_cast<NovaInt*>(rounded.get());
			if (nova_i->CNum() == 0) {
				PushError("Cannot divide by 0", node);
				return null;
			}
		}
	}

	if (node->op == "-") {
		op = NovaValue::Minus;
	}
	else if (node->op == "*") {
		op = NovaValue::Multiply;
	}
	else if (node->op == "/") {
		op = NovaValue::Divide;
	}
	else if (node->op == ">") {
		op = NovaValue::GreaterThen;
	}
	else if (node->op == "<") {
		op = NovaValue::LesserThen;
	}
	else if (node->op == ">=") {
		op = NovaValue::GreaterEqual;
	}
	else if (node->op == "<=") {
		op = NovaValue::LesserEqual;
	}
	else if (node->op == "==") {
		op = NovaValue::Equality;
	}
	else if (node->op == "%") {
		op = NovaValue::Mod;
	}

	std::shared_ptr<NovaValue> result = lhs->PerformOp(rhs, op);
	if (result) {
		literal_stack.push_back(result);
		return result;
	}
	else {
		PushError("Failed to perform operation of (" + lhs->Type() + NovaValue::OpToString(op) + rhs->Type() + ")", node);
		return null;
	}

}

ee_decl(FuncCallNode* node) {
	std::shared_ptr<NovaValue> func = EvaluateExpression(node->callee);
	if (func) {
		if (func->Type() == "Function") {
			NovaFunction* fn = static_cast<NovaFunction*>(func.get());
			std::vector<std::shared_ptr<NovaValue>> args;
			if (fn->this_qualified) {
				args.push_back(last_object);
			}
			for (ExprNode* arg : node->args) {
				std::shared_ptr<NovaValue> a_value = nullptr;
				a_value = EvaluateExpression(arg);
				if (a_value) {
					args.push_back(a_value);
				}

			}
			std::shared_ptr<NovaValue> result = fn->Call(args);
			if (!result) {
				result = null;
			}
			literal_stack.push_back(result);
			return result;
		}
		else {
			PushError(node->callee->Print() + " is not a function", node);
		}
	}
	else {
		for (std::pair<TypeDeclNode*, FuncDeclNode*> types : nova_types) {
			if (types.first->type_name == node->callee->Print()) {
				Scope this_obj(scope);
				scope = &this_obj;
				for (StmtNode* stmt : types.first->definition) {
					EvaluateStatement(stmt);
				}
				scope = this_obj.parent;
				std::shared_ptr<NovaValue> result = std::make_shared<NovaObject>();
				for (const std::pair<std::string, std::shared_ptr<NovaValue>>& pair : this_obj.variables) {
					result->accessables->insert(pair);
				}
				if (types.second) {
					PushScope();
					scope->LimitedSet("_NOVA_THIS", result);
					EvaluateStatement(types.second);
					EvaluateExpression(node);
					PopScope();
				}
				
				literal_stack.push_back(result);
				return result;
			}
		}
		PushError("Function (" + node->callee->Print() + ") not found in scope", node);
		return null;
	}
}

ee_decl(TernaryNode* node) {
	std::shared_ptr<NovaValue> val = EvaluateExpression(node->expression);

	if (val->Type() == "bool") {
		NovaBool* nb = static_cast<NovaBool*>(val.get());
		if (nb->CB()) {
			return EvaluateExpression(node->truthy_value);
		}
		else {
			return EvaluateExpression(node->falsey_value);
		}
	}
	else {
		PushError("Ternary expression does not evaluate to boolean type", node);
		return null;
	}
}

ee_decl(IntLiteralNode* node) {
	std::shared_ptr<NovaValue> val = std::make_shared<NovaInt>(node->number);
	literal_stack.push_back(val);
	return val;
}

ee_decl(FloatLiteralNode* node) {
	std::shared_ptr<NovaValue> val = std::make_shared<NovaFloat>(node->number);
	literal_stack.push_back(val);
	return val;
}

ee_decl(BoolLiteralNode* node) {
	std::shared_ptr<NovaValue> val = std::make_shared<NovaBool>(node->value);
	literal_stack.push_back(val);
	return val;
}

ee_decl(StringLiteralNode* node) {
	std::shared_ptr<NovaValue> val = std::make_shared<NovaString>(node->string);
	literal_stack.push_back(val);
	return val;
}

ee_decl(ArrayLiteralNode* node) {
	std::vector<std::shared_ptr<NovaValue>> arr;
	for (ExprNode* expr : node->values) {
		std::shared_ptr<NovaValue> val = EvaluateExpression(expr);
		arr.push_back(val);
	}
	std::shared_ptr<NovaValue> narr = std::make_shared<NovaArray>(arr);
	literal_stack.push_back(narr);
	return narr;
}

ee_decl(DotAccessNode* node) {
	std::shared_ptr<NovaValue> val = EvaluateExpression(node->left);
	// val.func()
	PushScope();
	for (const std::pair<std::string, std::shared_ptr<NovaValue>>& pair : val->GetFullAccessableList()) {
		scope->LimitedSet(pair.first, pair.second);
	}
	if (scope->LimitedHas("_NOVA_THIS")) { // if the object has the "this" field we pass it up the chain
		last_object = val;
	}
	std::shared_ptr<NovaValue> result = EvaluateExpression(node->right);
	
	PopScope();
	return result;
}

ee_decl(ArrayAccessNode* node) {
	std::shared_ptr<NovaValue> arr = EvaluateExpression(node->arr);
	std::shared_ptr<NovaValue> index = EvaluateExpression(node->index);

	if (arr->Type() == "Array") {
		NovaArray* narr = static_cast<NovaArray*>(arr.get());
		std::vector<std::shared_ptr<NovaValue>>& list = *narr->Arr();
		if (index->Type() == "Int") {
			NovaInt* nint = static_cast<NovaInt*>(index.get());
			return list[nint->CNum()];
		}
		else {
			PushError("Index is not an int", node);
			return null;
		}
	}
	else {
		PushError("Cannot use array access on a non array", node);
		return null;
	}
}

ee_decl(NullLiteralNode*) {
	return null;
}

ee_decl(NotNode* node) {
	if (node->expression) {
		std::shared_ptr<NovaValue> val = EvaluateExpression(node->expression);
		if (val->Type() == "bool") {
			NovaBool* nb = static_cast<NovaBool*>(val.get());
			std::shared_ptr<NovaValue> notnb = std::make_shared<NovaBool>(!nb->CB());
			literal_stack.push_back(notnb);
			return notnb;
		}
		if (val->Type() == "Float") {
			NovaFloat* nf = static_cast<NovaFloat*>(val.get());
			std::shared_ptr<NovaValue> notnf = std::make_shared<NovaFloat>(-nf->CNum());
			literal_stack.push_back(notnf);
			return notnf;
		}
		if (val->Type() == "Int") {
			NovaInt* nf = static_cast<NovaInt*>(val.get());
			std::shared_ptr<NovaValue> notnf = std::make_shared<NovaInt>(-nf->CNum());
			literal_stack.push_back(notnf);
			return notnf;
		}
		else {
			PushError("Unexpected type(" + val->Type() + ") after unary operator", node);
		}
	}
	else {
		PushError("Expected expression after unary operator", node);
		return null;
	}
}

ee_decl(IsNode* node) {
	std::shared_ptr<NovaValue> value = EvaluateExpression(node->lhs);
	std::shared_ptr<NovaValue> str = EvaluateExpression(node->rhs);

	if (str->Type() == "String") {
		bool is = value->Type() == str->ToString();
		std::shared_ptr<NovaValue> result = std::make_shared<NovaBool>(is);
		literal_stack.push_back(result);
		return result;
	}
	else {
		PushError("Expected String in type check", node);
	}

}
