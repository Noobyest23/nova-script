#include "../NovaScript/Parser/Optimizer.h"
#include "../NovaScript/ASTNodes/ProgramNode.h"
#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Value/Value.h"
#include "../NovaScript/Interpretor/Scope.h"

ProgramNode* Optimizer::Optimize() {
	return old_program;
}
