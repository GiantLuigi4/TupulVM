#include "Locals.h"
#include "Types.h"

#include <stdlib.h>

#include "Utils.h"

void popStack(Locals* locals) {
	int i = locals->pushPoints[locals->pushPoints.size() - 1];
	i = locals->stack.size() - i;
	locals->pushPoints.pop_back();
	int i1 = locals->stack.size();
	for (int indx = 0; indx < i; indx++) {
		i1 -= 1;
		TupulByte* bytes = locals->stack[i1];
		TupulByte* type = locals->stackTypes[i1];
		trackedFree(bytes);
		freeType(type);
		locals->stack.pop_back();
		locals->stackTypes.pop_back();
	}
}

void freeLocals(Locals* locals) {
	for (TupulByte* bytes : locals->stack) trackedFree(bytes);
	for (TupulByte* bytes : locals->locals) trackedFree(bytes);
	for (TupulByte* bytes : locals->stackTypes) freeType(bytes);
	for (TupulByte* bytes : locals->localTypes) freeType(bytes);
	locals->stack.clear();
	locals->stackTypes.clear();
	locals->locals.clear();
	locals->localTypes.clear();
	locals->pushPoints.clear();
	trackedFree(locals);
}
