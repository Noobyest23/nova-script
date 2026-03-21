#include "../NovaScript/Interpretor/Interpretor.h"

#define ee_decl(type) Value Interpretor::EvaluateExpression(type)
#define eep_decl(type) Value* Interpretor::EvaluateExpressionPtr(type)
#define ee(type) if (type n = dynamic_cast<type>(node)) {return EvaluateExpression(n);}
#define eep(type) if (type n = dynamic_cast<type>(node)) {return EvaluateExpressionPtr(n);}

#define STR(type) #type

ee_decl(ExprNode* node) {
	if (!node) {
		return nullval;
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
	ee(Vector2LiteralNode*)
	ee(Vector3LiteralNode*)
	ee(Vector4LiteralNode*)
	ee(ArrayAccessNode*)
	ee(AssignmentNode*)
	ee(CompoundOp*)
	ee(NullLiteralNode*)
	ee(NotNode*)
	PushError("Unrecognized Expression " + node->Print() + " (this expression has returned null)");
	return Value();
}

ee_decl(VariableNode* node) {
	Value* val = scope->Get(node->identifier);
	if (val) {
		return *val;
	}
	else {
		return Value();
	}
}

ee_decl(AssignmentNode* node) {
	Value* lhs = EvaluateExpressionPtr(node->left);
	if (!lhs || lhs->IsFunction() || lhs->is_manually_created) {
		PushError("Left side is not a modifiable value " + node->Print());
		if (lhs) {
			lhs->Release();
		}
		return Value();
	}

	Value rhs = EvaluateExpression(node->right);
	if (lhs->IsCPP()) {
		try {
			if (lhs->IsString()) {
				std::string& string = lhs->GetString();
				string = rhs.GetString();
			}
			else if (lhs->IsBool()) {
				bool& b = lhs->GetBool();
				b = rhs.GetBool();
			}
			else if (lhs->IsInt()) {
				int& i = lhs->GetInt();
				i = rhs.GetInt();
			}
			else if (lhs->IsFloat()) {
				float& f = lhs->GetFloat();
				f = rhs.GetFloat();
			}
			else if (lhs->IsVec2()) {
				auto& v = lhs->GetVec2();
				v = rhs.GetVec2();
			}
			else if (lhs->IsVec3()) {
				auto& v = lhs->GetVec3();
				v = rhs.GetVec3();
			}
			else if (lhs->IsVec4()) {
				auto& v = lhs->GetVec4();
				v = rhs.GetVec4();
			}
		}
		catch (std::exception e) {
			PushError("Trying to assign " + rhs.Type() + " to a non-matching cpp variable (" + lhs->Type() + ")");
		}
		return *lhs;
	}
	lhs->data = rhs.data;
	return *lhs;
}

#define iiop(_op) if (node->op == STR(_op)) {val _op rhs.GetInt();}
#define ifop(_op) if (node->op == STR(_op)) {val _op static_cast<int>(rhs.GetFloat());}
#define fiop(_op) if (node->op == STR(_op)) {val _op rhs.GetInt();}
#define ffop(_op) if (node->op == STR(_op)) {val _op rhs.GetFloat();}
#define bop(_op) if (node->op == STR(_op)) {val _op rhs.GetBool();}

ee_decl(CompoundOp* node) {
	// Set a value
	Value* lhs = EvaluateExpressionPtr(node->lhs);
	if (!lhs) {
		PushError("Left side is not a modifyable value " + node->Print());
		return Value();
	}
	Value rhs = EvaluateExpression(node->rhs);

	try {
		if (lhs) {
			if (lhs->IsInt()) {
				int& val = lhs->GetInt();
				if (rhs.IsInt()) {
					iiop(+= );
					iiop(-= );
					iiop(*= );
					iiop(/= );
					return *lhs;
				}
				else if (rhs.IsFloat()) {
					ifop(+= );
					ifop(-= );
					ifop(*= );
					ifop(/= );
					return *lhs;
				}
				return *lhs;
			}
			if (lhs->IsFloat()) {
				float& val = lhs->GetFloat();
				if (rhs.IsInt()) {
					fiop(+= );
					fiop(-= );
					fiop(*= );
					fiop(/= );
					return *lhs;
				}
				else if (rhs.IsFloat()) {
					ffop(+= );
					ffop(-= );
					ffop(*= );
					ffop(/= );
					return *lhs;
				}
				return *lhs;
			}
			if (lhs->IsString()) {
				std::string& val = lhs->GetString();
				if (node->op == "+=") {
					val += rhs.GetString();
				}
				else {
					PushError("Cannot perform " + node->op + " on a string");
				}

				return *lhs;
			}

			throw std::exception((std::string("Cannot perform a compound op on a variable of type ") + lhs->Type()).c_str());
		}
		else {
			PushError("Cannot assign to an expression of this type" + node->Print());
			return nullval;
		}
	}
	catch (std::exception e) {
		PushError(e.what() + '\n' + lhs->ToString() + '\n' + rhs.ToString());
		return nullval;
	}
}

#undef iiop
#undef ifop
#undef ffop
#undef fiop
#undef bop

#define iiop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetInt() _op rhs.GetInt());}
#define ifop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetInt() _op rhs.GetFloat());}
#define ffop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetFloat() _op rhs.GetFloat());}
#define fiop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetFloat() _op rhs.GetInt());}
#define bop(_op) if (node->op == STR(_op)) {r_value = Value(lhs.GetBool() _op rhs.GetBool());}

ee_decl(OpNode* node) {
	Value lhs = EvaluateExpression(node->left);
	Value rhs = EvaluateExpression(node->right);

	Value r_value;

	try {
		
		if (lhs.IsInt()) {
			if (rhs.IsInt()) {
				iiop(+);
				iiop(-);
				iiop(*);
				iiop(/);
				iiop(>);
				iiop(<);
				iiop(>=);
				iiop(<=);
				iiop(==);
				return r_value;
			}
			else if (rhs.IsFloat()) {
				ifop(+);
				ifop(-);
				ifop(*);
				ifop(/);
				ifop(>);
				ifop(<);
				ifop(>=);
				ifop(<=);
				ifop(==);
				return r_value;
			}
			PushError("Cannot add type to a int " + node->Print());
			return nullval;
		}
		if (lhs.IsFloat()) {
			if (rhs.IsInt()) {
				fiop(+);
				fiop(-);
				fiop(*);
				fiop(/);
				fiop(>);
				fiop(<);
				fiop(>=);
				fiop(<=);
				fiop(==);
				return r_value;
			}
			else if (rhs.IsFloat()) {
				ffop(+);
				ffop(-);
				ffop(*);
				ffop(/);
				ffop(>);
				ffop(<);
				ffop(>=);
				ffop(<=);
				ffop(==);
				return r_value;
			}
			PushError("Cannot add type to a float " + node->Print());
			return nullval;
		}
		if (lhs.IsString()) {
			if (node->op == "+") {
				r_value = Value(lhs.GetString() + rhs.GetString());
				return r_value;
			}
			else {
				PushError("Cannot perform " + node->op + " on a string");
				return nullval;
			}
		}
		if (lhs.IsBool()) {
			if (rhs.IsBool()) {
				bop(==);
				bop(&&);
				bop(||);
				return r_value;
			}
			else {
				PushError("Cannot perform boolean operations with type " + rhs.Type());
			}
		}
		if (lhs.IsObj()) {
			if (lhs.IsCPP()) {
				CPPObject& obj = std::get<CPPObject>(lhs.data);
				if (obj.ptr) {
					return Value(true);
				}
				else {
					return Value(false);
				}
			}
			else {
				NovaType& type = std::get<NovaType>(lhs.data);
				if (type.GetAll().size() != 0) {
					return Value(true);
				}
				else {
					return Value(false);
				}
			}
		}
		if (lhs.data.index() == 0) {
			// null
			return Value(false);
		}
		throw std::exception((std::string("Cannot perform an op on a variable of type ") + lhs.Type()).c_str());
	}
	catch (std::exception e) {
		PushError((std::string("Cannot perform an op on a variable of type ") + lhs.Type()).c_str());
		return nullval;
	}
}

ee_decl(FuncCallNode* node) {
	Value* func = scope->Get(node->func_id);
	if (func) {
		if (func->IsFunction()) {
			std::vector<Value*> args;
			if (scope->Has("this")) {
				args.push_back(scope->Get("this"));
			}
			for (ExprNode* arg : node->args) {
				Value* a_value = EvaluateExpressionPtr(arg);
				if (a_value) {
					args.push_back(a_value);
				}
				else {
					a_value = new Value(EvaluateExpression(arg));
					a_value->is_manually_created = true;
					args.push_back(a_value);
				}
			}
			return Call(node->func_id, args);
		}
		else {
			PushError(node->func_id + " is not a function");
			return nullval;
		}
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
				return Value(object);
			}
		}
		PushError("Function (" + node->func_id + ") not found in scope");
		return nullval;
	}
}

ee_decl(TernaryNode* node) {
	Value val = EvaluateExpression(node->expression);

	if (val.IsBool()) {
		if (val.GetBool()) {
			return EvaluateExpression(node->truthy_value);
		}
		else {
			return EvaluateExpression(node->falsey_value);
		}
	}
	else {
		PushError("Ternary expression does not evaluate to boolean type " + node->Print());
		return nullval;
	}
}

ee_decl(IntLiteralNode* node) {
	return Value(node->number);
}

ee_decl(FloatLiteralNode* node) {
	return Value(node->number);
}

ee_decl(BoolLiteralNode* node) {
	return Value(node->value);
}

ee_decl(StringLiteralNode* node) {
	return Value(node->string);
}

ee_decl(ArrayLiteralNode* node) {
	std::vector<Value> arr;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		arr.push_back(val);
	}
	return Value(arr);
}

ee_decl(DotAccessNode* node) {
	Value* obj = EvaluateExpressionPtr(node->left);
	Value* left = nullptr;

	if (!obj or obj->data.index() == 0) {
		PushError("Could not resolve this obj " + node->Print());
		return Value();
	}

	Scope* n_scope = nullptr;
	if (obj->IsCPP() and not obj->IsVec()) {
		n_scope = &std::get<CPPObject>(obj->data).scope;
	}
	else if (not obj->IsVec()){
		n_scope = &std::get<Scope>(obj->data);
	}

	if (FuncCallNode* func = dynamic_cast<FuncCallNode*>(node->right)) {
		left = n_scope->Get(func->func_id);
		if (!left) {
			PushError("Could not find function " + func->func_id + " in " + node->left->Print() + "'s scope");
			return Value();
		}
		if (left->IsFunction()) {
			std::vector<Value*> args;
			if (obj->IsCPP()) {
				args.push_back(obj);
			}
			for (ExprNode* arg : func->args) {
				Value* a_value = EvaluateExpressionPtr(arg);
				if (a_value) {
					args.push_back(a_value);
				}
				else {
					a_value = new Value(EvaluateExpression(arg));
					a_value->is_manually_created = true;
					args.push_back(a_value);
				}
			}
			if (left->IsCPP()) {
				CPPFunction& func = std::get<CPPFunction>(left->data);
				Value r_value = func(args);
				for (Value* arg : args) {
					arg->Release();
				}
				obj->Release();
				return r_value;
			}
			else {
				NovaFunction& func = std::get<NovaFunction>(left->data);
				Scope* p_scope = scope;
				scope = n_scope;

				for (int i = 0; i < func->args.size(); i++) {
					scope->Set(func->args[i], *args[i]);
				}

				for (StmtNode* stmt : func->body) {
					EvaluateStatement(stmt);
					if (return_flag) {
						return_flag = false;
						scope = p_scope;
						for (Value* arg : args) {
							arg->Release();
						}
						obj->Release();
						return return_val;
					}
				}
				scope = p_scope;
				for (Value* arg : args) {
					arg->Release();
				}
				obj->Release();
				return Value();

			}
		}
		else {
			PushError(func->func_id + "is not a callable");
			obj->Release();
			return nullval;
		}
	}
	else if (VariableNode* var = dynamic_cast<VariableNode*>(node->right)) {
		
		if (obj->IsVec()) {
			if (obj->IsVec2()) {
				glm::vec2& o_vec = obj->GetVec2();
				if (var->identifier == "x") {
					obj->Release();
					return Value(o_vec.x);
				}
				else if (var->identifier == "y") {
					obj->Release();
					return Value(o_vec.y);
				}
				else {
					PushError(var->identifier + "cannot be found on vec2");
					obj->Release();
					return Value();
				}
			}
			if (obj->IsVec3()) {
				glm::vec3& o_vec = obj->GetVec3();
				if (var->identifier == "x") {
					obj->Release();
					return Value(o_vec.x);
				}
				else if (var->identifier == "y") {
					obj->Release();
					return Value(o_vec.y);
				}
				else if (var->identifier == "z") {
					obj->Release();
					return Value(o_vec.z);
				}
				else {
					PushError(var->identifier + "cannot be found on vec3");
					obj->Release();
					return Value();
				}
			}
			if (obj->IsVec4()) {
				glm::vec4& o_vec = obj->GetVec4();
				if (var->identifier == "x") {
					obj->Release();
					return Value(o_vec.x);
				}
				else if (var->identifier == "y") {
					obj->Release();
					return Value(o_vec.y);
				}
				else if (var->identifier == "z") {
					obj->Release();
					return Value(o_vec.z);
				}
				else if (var->identifier == "w") {
					obj->Release();
					return Value(o_vec.w);
				}
				else {
					PushError(var->identifier + "cannot be found on vec4");
					obj->Release();
					return Value();
				}
			}

		}

		obj->Release();
		Value* val = n_scope->Get(var->identifier);
		if (val) {
			return *val;
		}
		else {
			return Value();
		}
	}
	else {
		Scope* p_scope = scope;
		scope = n_scope;
		Value val = EvaluateExpression(node->right);
		obj->Release();
		scope = p_scope;
		return val;
	}

	PushError("Invalid dot access " + node->Print());
	obj->Release();
	return nullval;
}

ee_decl(Vector2LiteralNode* node) {
	std::vector<Value> values;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		if (val.IsInt() or val.IsFloat() or val.IsVec()) {
			values.push_back(val);
		}
		else {
			PushError("Cannot assign " + val.Type() + " to a Vector");
			return nullval;
		}
	}

	switch (values.size()) {
	case 1: {
		if (values[0].data.index() == 12) {
			return glm::vec2(values[0].GetVec2());
		}
		else if (values[0].IsNum()) {
			return glm::vec2(values[0].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector2");
		return nullval;
		break;
	}
	case 2: {
		if (values[0].IsNum() and values[1].IsNum()) {
			return glm::vec2(values[0].GetNum(), values[1].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector2");
		return nullval;
		break;
	}
	}
	PushError("Invalid vector construction size" + node->Print());
	return nullval;
}

ee_decl(Vector3LiteralNode* node) {
	std::vector<Value> values;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		if (val.IsInt() or val.IsFloat() or val.IsVec()) {
			values.push_back(val);
		}
		else {
			PushError("Cannot assign " + val.Type() + " to a Vector");
			return nullval;
		}
	}

	switch (values.size()) {
	case 1: {
		if (values[0].data.index() == 13) {
			return glm::vec3(values[0].GetVec3());
		}
		else if (values[0].IsNum()) {
			return glm::vec3(values[0].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector3");
		return nullval;
		break;
	}
	case 3: {
		if (values[0].IsNum() and values[1].IsNum() and values[2].IsNum()) {
			return glm::vec3(values[0].GetNum(), values[1].GetNum(), values[2].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector3");
		return nullval;
		break;
	}
	}
	PushError("Invalid vector construction size" + node->Print());
	return nullval;
}

ee_decl(Vector4LiteralNode* node) {
	std::vector<Value> values;
	for (ExprNode* expr : node->values) {
		Value val = EvaluateExpression(expr);
		if (val.IsInt() or val.IsFloat() or val.IsVec()) {
			values.push_back(val);
		}
		else {
			PushError("Cannot assign " + val.Type() + " to a Vector");
			return nullval;
		}
	}

	switch (values.size()) {
	case 1: {
		if (values[0].data.index() == 14) {
			return glm::vec4(values[0].GetVec4());
		}
		else if (values[0].IsNum()) {
			return glm::vec4(values[0].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector4");
		return nullval;
		break;
	}
	case 3: {
		if (values[0].IsNum() and values[1].IsNum() and values[2].IsNum() and values[3].IsNum()) {
			return glm::vec4(values[0].GetNum(), values[1].GetNum(), values[2].GetNum(), values[3].GetNum());
		}
		PushError("Attempt to assign " + values[0].Type() + " as only argument to Vector4");
		return nullval;
		break;
	}
	}
	PushError("Invalid vector construction size" + node->Print());
	return nullval;
}

ee_decl(ArrayAccessNode* node) {
	Value arr = EvaluateExpression(node->arr);
	Value index = EvaluateExpression(node->index);

	if (arr.IsArray()) {
		std::vector<Value>& list = arr.GetArray();
		if (index.IsInt()) {
			return list[index.GetInt()];
		}
		else {
			PushError("Index is not an int" + node->Print());
			return nullval;
		}
	}
	else {
		PushError("Cannot use array access on a non array " + node->Print());
		return nullval;
	}
}

ee_decl(NullLiteralNode*) {
	return Value();
}

ee_decl(NotNode* node) {
	if (node->expression) {
		Value val = EvaluateExpression(node->expression);
		if (val.IsBool()) {
			return Value(!val.GetBool());
		}
		if (val.IsFloat()) {
			return Value(-val.GetFloat());
		}
		if (val.IsInt()) {
			return Value(-val.GetInt());
		}
	}
	else {
		PushError("Expected expression after unary operator" + node->Print());
		return nullval;
	}
}

// If we are trying to modify a place in memory then we want a pointer to a value instead of just a value
// 
// EvaluateExpressionPtr
// 



eep_decl(ExprNode* node) {
	eep(VariableNode*)
	eep(TernaryNode*)
	eep(DotAccessNode*)
	
	// if the pointer cannot be obtained we must manually create and destroy the object
	Value* val = new Value(EvaluateExpression(node));
	val->is_manually_created = true;
	return val;
}

eep_decl(VariableNode* node) {
	return scope->Get(node->identifier);
}

eep_decl(TernaryNode* node) {
	Value val = EvaluateExpression(node->expression);

	if (val.IsBool()) {
		if (val.GetBool()) {
			return EvaluateExpressionPtr(node->truthy_value);
		}
		else {
			return EvaluateExpressionPtr(node->falsey_value);
		}
	}
	else {
		PushError("Ternary expression does not evaluate to boolean type " + node->Print());
		return &nullval;
	}
}

eep_decl(DotAccessNode* node) {
	Value* obj = EvaluateExpressionPtr(node->left);
	Value* left = nullptr;

	Scope* n_scope = nullptr;
	if (obj->IsCPP() and not obj->IsVec()) {
		n_scope = &std::get<CPPObject>(obj->data).scope;
	}
	else if (not obj->IsVec()) {
		n_scope = &std::get<Scope>(obj->data);
	}

	if (FuncCallNode* func = dynamic_cast<FuncCallNode*>(node->right)) {
		left = n_scope->Get(func->func_id);
		if (!left) {
			PushError("Could not find function " + func->func_id + " in " + node->left->Print() + "'s scope");
			return nullptr;
		}
		if (left->IsFunction()) {
			std::vector<Value*> args;
			if (obj->IsCPP()) {
				args.push_back(obj);
			}
			for (ExprNode* arg : func->args) {
				args.push_back(EvaluateExpressionPtr(arg));
			}

			if (left->IsCPP()) {
				CPPFunction& func = std::get<CPPFunction>(left->data);
				Value* r_value = new Value(func(args));
				for (Value* arg : args) {
					arg->Release();
				}
				r_value->is_manually_created = true;
				return r_value;
			}
			else {
				NovaFunction& func = std::get<NovaFunction>(left->data);
				Scope* p_scope = scope;
				scope = n_scope;

				for (int i = 0; i < func->args.size(); i++) {
					scope->Set(func->args[i], *args[i]);
				}

				for (StmtNode* stmt : func->body) {
					EvaluateStatement(stmt);
					if (return_flag) {
						return_flag = false;
						scope = p_scope;
						for (Value* arg : args) {
							arg->Release();
						}
						return &return_val;
					}
				}
				scope = p_scope;
				for (Value* arg : args) {
					arg->Release();
				}
				Value* null_val = new Value();
				null_val->is_manually_created = true;
				return null_val;

			}
		}
	}
	else if (VariableNode* var = dynamic_cast<VariableNode*>(node->right)) {

		if (obj->IsVec()) {
			if (obj->IsVec2()) {
				glm::vec2& o_vec = obj->GetVec2();
				if (var->identifier == "x") {
					Value* ret = new Value(CPPVariable(o_vec.x));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else if (var->identifier == "y") {
					Value* ret = new Value(CPPVariable(o_vec.y));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else {
					PushError(var->identifier + "cannot be found on vec2");
					obj->Release();
					return nullptr;
				}
			}
			if (obj->IsVec3()) {
				glm::vec3& o_vec = obj->GetVec3();
				if (var->identifier == "x") {
					Value* ret = new Value(CPPVariable(o_vec.x));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else if (var->identifier == "y") {
					Value* ret = new Value(CPPVariable(o_vec.y));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else if (var->identifier == "z") {
					Value* ret = new Value(CPPVariable(o_vec.z));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else {
					PushError(var->identifier + "cannot be found on vec3");
					obj->Release();
					return nullptr;
				}
			}
			if (obj->IsVec4()) {
				glm::vec4& o_vec = obj->GetVec4();
				if (var->identifier == "x") {
					Value* ret = new Value(CPPVariable(o_vec.x));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else if (var->identifier == "y") {
					Value* ret = new Value(CPPVariable(o_vec.y));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else if (var->identifier == "z") {
					Value* ret = new Value(CPPVariable(o_vec.z));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else if (var->identifier == "w") {
					Value* ret = new Value(CPPVariable(o_vec.w));
					ret->is_manually_created = true;
					obj->Release();
					return ret;
				}
				else {
					PushError(var->identifier + "cannot be found on vec4");
					obj->Release();
					return nullptr;
				}
			}

		}

		obj->Release();
		return left = n_scope->Get(var->identifier);
	}
	else {
		Scope* p_scope = scope;
		scope = n_scope;
		Value* val = EvaluateExpressionPtr(node->right);
		obj->Release();
		scope = p_scope;
		if (val) {
			return val;
		}
		else {
			PushError("Invalid dot access" + node->Print());
			return val;
		}
	}
	PushError("Invalid dot access");
	return &nullval;
}

eep_decl(ArrayAccessNode* node) {
	Value* arr = EvaluateExpressionPtr(node->arr);
	Value index = EvaluateExpression(node->index);

	if (arr->IsArray()) {
		std::vector<Value>& list = arr->GetArray();
		if (index.IsInt()) {
			arr->Release();
			return &list[index.GetInt()];
		}
		else {
			PushError("Index is not an int" + node->Print());
			arr->Release();
			return &nullval;
		}
	}
	else {
		PushError("Cannot use array access on a non array" + node->Print());
		arr->Release();
		return &nullval;
	}

}
