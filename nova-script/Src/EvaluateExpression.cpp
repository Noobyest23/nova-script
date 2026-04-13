#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Interpretor/Scope.h"
#include "../NovaScript/Value/FunctionValue.h"
#include "../NovaScript/Value/BoolValue.h"
#include "../NovaScript/Value/IntValue.h"
#include "../NovaScript/Value/FloatValue.h"
#include "../NovaScript/Value/StringValue.h"
#include "../NovaScript/Value/ArrayValue.h"
#include "../NovaScript/Value/NovaObject.h"

#undef ee
#define ee_decl(type) NovaValue* Interpretor::EvaluateExpression(type)
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
		NovaValue* val = scope->Get(node->identifier);
		return val;
	}
	else {
		PushError("Variable, " + node->identifier + " does not exist in the scope", node);
		return null;
	}
}

ee_decl(AssignmentNode* node) {
	NovaValue* lhs = EvaluateExpression(node->left);

	NovaValue* rhs = nullptr;
	if (VariableNode* v = dynamic_cast<VariableNode*>(node->right)) {
		if (v->as_ptr) {
			rhs = scope->Get(v->identifier);
			rhs->AddRef();
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
			if (VariableNode* v = dynamic_cast<VariableNode*>(node->right)) {
				if (not v->as_ptr) {
					rhs->Release();
				}
			}
		}
		return lhs;
	}
	else if (DotAccessNode* dot = dynamic_cast<DotAccessNode*>(node->left)) {
		if (VariableNode* var = dynamic_cast<VariableNode*>(dot->right)) {
			if (!lhs->Assign(rhs)) {
				lhs = EvaluateExpression(dot->left); // gives us the var in var.property
				
				
				lhs->accessables->at(var->identifier)->Release();
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
				NovaArray* narr = static_cast<NovaArray*>(lhs);

				NovaValue* index = EvaluateExpression(arr->index);
				if (!index or index->Type() != "Int") { PushError("Index in array assignment is not an int", node); return null; }
				NovaInt* int_index = static_cast<NovaInt*>(index);
				if (int_index->CNum() > narr->CArr().size()) { PushError("Out of bounds array assignment", node); return null; }


				(*narr->Arr())[int_index->CNum()]->Release();
				(*narr->Arr())[int_index->CNum()] = rhs;
				return narr->CArr()[int_index->CNum()];
				
			}
			return lhs;
		}
	}
	
}

ee_decl(CompoundOp* node) {
	NovaValue* lhs = EvaluateExpression(node->lhs);
	NovaValue* rhs = EvaluateExpression(node->rhs);

	NovaValue::NovaOperator op = NovaValue::CompoundPlus;

	if (node->op == "-=") {
		op = NovaValue::CompoundMinus;
	}
	else if (node->op == "*=") {
		op = NovaValue::CompoundMultiply;
	}
	else if (node->op == "/=") {
		op = NovaValue::CompoundDivide;
	}

	NovaValue* result = lhs->PerformCompoundOp(rhs, op);
	return result;
}

ee_decl(OpNode* node) {
	NovaValue* lhs = EvaluateExpression(node->left);
	NovaValue* rhs = EvaluateExpression(node->right);

	NovaValue::NovaOperator op = NovaValue::Plus;

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

	NovaValue* result = lhs->PerformOp(rhs, op);
	literal_stack.push_back(result);
	return result;

}

ee_decl(FuncCallNode* node) {
	NovaValue* func = scope->Get(node->func_id);
	if (!func) { PushError(node->func_id + " was not found in scope"); return null; }
	if (func->Type() == "NovaFunction") {
		NovaFunction* fn = static_cast<NovaFunction*>(func);
		std::vector<NovaValue*> args;
		if (fn->this_qualified and scope->LimitedHas("_NOVA_THIS")) {
			args.push_back(scope->LimitedGet("_NOVA_THIS"));
		}
		for (ExprNode* arg : node->args) {
			NovaValue* a_value = nullptr;
			a_value = EvaluateExpression(arg);
			if (a_value) {
				a_value->AddRef();
				args.push_back(a_value);
			}
			
		}
		NovaValue* result = fn->Call(args);
		if (!result) {
			result = null;
		}
		for (NovaValue* arg : args) {
			if (scope->Get("_NOVA_THIS") != arg) {
				arg->Release();
			}
		}
		literal_stack.push_back(result);
		return result;
	}
	else {
		for (std::pair<TypeDeclNode*, FuncDeclNode*> types : nova_types) {
			if (types.first->type_name == node->func_id) {
				Scope object;
				Scope* p_scope = scope;
				scope = &object;
				for (StmtNode* declNode : types.first->definition) {
					EvaluateStatement(declNode);
				}
				if (types.second) {
					EvaluateExpression(node);
				}
				scope = p_scope;
				NovaObject* result = new NovaObject();
				for (const std::pair<std::string, NovaValue*>& pair : object.variables) {
					pair.second->AddRef();
					result->accessables->insert(pair);
				}
				literal_stack.push_back(result);
				return result;
			}
		}
		PushError("Function (" + node->func_id + ") not found in scope", node);
		return null;
	}
}

ee_decl(TernaryNode* node) {
	NovaValue* val = EvaluateExpression(node->expression);

	if (val->Type() == "bool") {
		NovaBool* nb = static_cast<NovaBool*>(val);
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
	NovaInt* val = new NovaInt(node->number);
	literal_stack.push_back(val);
	return val;
}

ee_decl(FloatLiteralNode* node) {
	NovaFloat* val = new NovaFloat(node->number);
	literal_stack.push_back(val);
	return val;
}

ee_decl(BoolLiteralNode* node) {
	NovaBool* val = new NovaBool(node->value);
	literal_stack.push_back(val);
	return val;
}

ee_decl(StringLiteralNode* node) {
	NovaString* val = new NovaString(node->string);
	literal_stack.push_back(val);
	return val;
}

ee_decl(ArrayLiteralNode* node) {
	std::vector<NovaValue*> arr;
	for (ExprNode* expr : node->values) {
		NovaValue* val = EvaluateExpression(expr);
		if (val) {
			val->AddRef();
		}
		arr.push_back(val);
	}
	NovaArray* narr = new NovaArray(arr);
	literal_stack.push_back(narr);
	return narr;
}

ee_decl(DotAccessNode* node) {
	NovaValue* val = EvaluateExpression(node->left);
	if (!val->accessables) { PushError("Left side of dot access does not have accessables", node); return null; }
	// val.func()
	Scope* p_scope = scope;
	Scope n_scope(p_scope);
	scope = &n_scope;
	for (const std::pair<std::string, NovaValue*>& pair : *val->accessables) {
		n_scope.LimitedSet(pair.first, pair.second);
	}
	if (n_scope.LimitedHas("_NOVA_THIS")) { // if the object has the "this" field we pass it up the chain
		n_scope.LimitedSet("_NOVA_THIS", val);
	}
	NovaValue* result = EvaluateExpression(node->right);
	
	scope = p_scope;
	return result;
}

ee_decl(ArrayAccessNode* node) {
	NovaValue* arr = EvaluateExpression(node->arr);
	NovaValue* index = EvaluateExpression(node->index);

	if (arr->Type() == "Array") {
		NovaArray* narr = static_cast<NovaArray*>(arr);
		std::vector<NovaValue*>& list = *narr->Arr();
		if (index->Type() == "Int") {
			NovaInt* nint = static_cast<NovaInt*>(index);
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
		NovaValue* val = EvaluateExpression(node->expression);
		if (val->Type() == "bool") {
			NovaBool* nb = static_cast<NovaBool*>(val);
			NovaBool* notnb = new NovaBool(!nb->CB());
			literal_stack.push_back(notnb);
			return notnb;
		}
		if (val->Type() == "Float") {
			NovaFloat* nf = static_cast<NovaFloat*>(val);
			NovaFloat* notnf = new NovaFloat(-nf->CNum());
			literal_stack.push_back(notnf);
			return notnf;
		}
		if (val->Type() == "Int") {
			NovaInt* nf = static_cast<NovaInt*>(val);
			NovaInt* notnf = new NovaInt(-nf->CNum());
			literal_stack.push_back(notnf);
			return notnf;
		}
		else {
			PushError("Unexpected value after unary operator", node);
		}
	}
	else {
		PushError("Expected expression after unary operator", node);
		return null;
	}
}

ee_decl(IsNode* node) {
	NovaValue* value = EvaluateExpression(node->lhs);
	NovaValue* str = EvaluateExpression(node->rhs);

	if (str->Type() == "String") {
		if (value->Type() == str->ToString()) {
			NovaBool* b = new NovaBool(true);
			literal_stack.push_back(b);
			return b;
		}
		else {
			NovaBool* b = new NovaBool(false);
			literal_stack.push_back(b);
			return b;
		}
	}
	else {
		PushError("Expected String in type check", node);
	}

}
