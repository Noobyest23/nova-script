#ifndef NOVASCRIPT_VECTOR_LIB_H
#define NOVASCRIPT_VECTOR_LIB_H

#include "../Interpretor/Scope.h"
#include "nova_std_macro.h"
#include "NovaModule.h"
#include "../Value/Vector2Value.h"
#include "../Value/FloatValue.h"
#include "../value/IntValue.h"

class NovaVectorModule : public NovaModule {
public:

	NovaVectorModule() {
		module_name = "vector";
	}

	nova_std_decl(Vector2) {
		if (args.size() > 1) {
			numfget(x, 0);
			numfget(y, 1);

			return new NovaVector2(x, y);
		}
		else if (args.size() > 0) {
			numfget(x, 0);
			
			return new NovaVector2(x, x);
		}
		else {
			return new NovaVector2(0.0f, 0.0f);
		}
	}

	

	

	NovaObject* GetModule() {
		NovaObject* obj = new NovaObject();
		obj->PushBack("Vector2", new NovaFunction(Vector2));
		return obj;
	}

};



#endif