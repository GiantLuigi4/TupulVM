#include "Locals.h"
#include "Types.h"

void popStack(Locals* locals) {
	int i = locals->pushPoints[locals->pushPoints.size() - 1];
	i = locals->stack.size() - i;
	locals->pushPoints.pop_back();
	for (int indx = 0; indx < i; indx++) {
		locals->stack.pop_back();
		locals->stackTypes.pop_back();
	}
}

#include <stdlib.h>

void freeLocals(Locals* locals) {
	for (byte* bytes : locals->stack) free(bytes);
	for (byte* bytes : locals->locals) free(bytes);
	for (byte* bytes : locals->stackTypes) freeType(bytes);
	for (byte* bytes : locals->localTypes) freeType(bytes);
	free(locals);
}
