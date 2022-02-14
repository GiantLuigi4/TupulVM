#define byte unsigned char

extern byte* BYTE;
extern byte* SHORT;
extern byte* HALF;
extern byte* INT;
extern byte* FLOAT;
extern byte* LONG;
extern byte* DOUBLE;

void freeType(byte* type); // frees only non-primitives, as those are the only things that aren't constants
// TODO: find better name
byte* copyType(byte* src); // copies the type if the type is a non-primitive, elsewise returns the source
short getTypeLength(byte* type);