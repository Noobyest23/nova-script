#include "../NovaScript/Interpretor/Interpretor.h"
#include "../NovaScript/Value/NullValue.h"

std::shared_ptr<NovaValue> NovaNull::Copy() const {
	return Interpretor::null;
}
