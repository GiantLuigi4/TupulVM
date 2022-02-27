#pragma once
#include "Definitions.h"

extern TupulByte* BYTE; // also a boolean, although booleans will be an interesting data type in field storage
extern TupulByte* SHORT;
extern TupulByte* CHAR;
extern TupulByte* HALF;
extern TupulByte* INT;
extern TupulByte* FLOAT;
extern TupulByte* LONG;
extern TupulByte* DOUBLE;

void freeType(TupulByte* type); // frees only non-primitives, as those are the only things that aren't constants
// TODO: find better name
TupulByte* copyType(TupulByte* src); // copies the type if the type is a non-primitive, elsewise returns the source
short getTypeLength(TupulByte* type);
TupulByte* tupCast(TupulByte* val, TupulByte* typeSrc, TupulByte* typeDst);
TupulByte* tupSum(TupulByte* num0, TupulByte* type0, TupulByte* num1, TupulByte* type1, TupulByte** type);
TupulByte* tupDiff(TupulByte* num0, TupulByte* type0, TupulByte* num1, TupulByte* type1, TupulByte** type);
TupulByte* preferredType(TupulByte* type0, TupulByte* type1);
