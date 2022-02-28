#include "STDClasses.h"
#include "Locals.h"

#include "Utils.h"


#include "Definitions.h"

#include "data/Types.h"

#include "stdlib/tupul/lang/System.h"

TupulClass* getSTDClass(char* name) {
	string namestr = name;
	if (namestr == "tupul.lang.System") return makeSystemClass();
	// TODO: learn freaking exceptions :wheeze:
	return {0};
}
