#define byte unsigned char

extern byte* BYTE; // also a boolean, although booleans will be an interesting data type in field storage
extern byte* SHORT;
extern byte* CHAR;
extern byte* HALF;
extern byte* INT;
extern byte* FLOAT;
extern byte* LONG;
extern byte* DOUBLE;

void freeType(byte* type); // frees only non-primitives, as those are the only things that aren't constants
// TODO: find better name
byte* copyType(byte* src); // copies the type if the type is a non-primitive, elsewise returns the source
short getTypeLength(byte* type);
byte* tupCast(byte* val, byte* typeSrc, byte* typeDst);
byte* tupSum(byte* num0, byte* type0, byte* num1, byte* type1, byte** type);
byte* preferredType(byte* type0, byte* type1);
