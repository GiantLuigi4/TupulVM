#include "Types.h"
#include "../Utils.h"

TupulByte* BYTE			=	(TupulByte[]) {0  };
TupulByte* SHORT		=	(TupulByte[]) {1  };
TupulByte* CHAR			=	(TupulByte[]) {2  };
TupulByte* HALF			=	(TupulByte[]) {3  };
TupulByte* INT			=	(TupulByte[]) {4  };
TupulByte* FLOAT		=	(TupulByte[]) {5  };
TupulByte* LONG			=	(TupulByte[]) {6  };
TupulByte* DOUBLE		=	(TupulByte[]) {7  };
TupulByte* ERR	 		=	(TupulByte[]) {253}; // can be caught, regular exception thrown by the code of the program being run
TupulByte* VM_ERR 		=	(TupulByte[]) {254}; // cannot be caught, as it is an error in the vm itself. likely to lead to memory leaks if it does get caught
TupulByte* UNDEFINED	=	(TupulByte[]) {255};

#include <stdlib.h>

void freeType(TupulByte* type) {
	if (type[0] == 8) trackedFree(type);
}

TupulByte* copyType(TupulByte* type) {
	if (type[0] == 8) {
		unsigned short len = ((type[1] & 0xFF) << 8) |
							 ((type[2] & 0xFF) << 0);
		TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), len);
		// for (int i = 0; i < len; i++) out[i] = type[i];
		memcpy(out, type,  len);
		return out;
	}
	return type;
}

unsigned short getTypeLength(TupulByte* type) {
	if (type[0] != 8) {
		// well that's neat
		switch (type[0]) {
			case 0: return 1;
			case 1: case 2: return 2;
			case 3: case 4: return 4;
			case 5: case 6: return 8;
			default: return -1;
		}
	} else {
		short len = ((type[1] & 0xFF) << 8) |
					((type[2] & 0xFF) << 0);
		return len;
	}
}

#include <cstdio>
#include <string.h>
TupulByte* tupCast(TupulByte* val, TupulByte* typeSrc, TupulByte* typeDst) {
	if (typeSrc == typeDst) {
		unsigned short len = getTypeLength(typeSrc);
		TupulByte* bytesCopy = (TupulByte*) trackedAlloc(sizeof(TupulByte), len);
		// for (unsigned short  i = 0; i < len; i++) bytesCopy[i] = val[i];
		memcpy(bytesCopy, val,  len);
		return bytesCopy;
	}
	// printf("%i\n", typeSrc[0]);
	// printf("%i\n", typeDst[0]);
	if (typeDst == BYTE) {
		TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 1);
		out[0] = val[0];
		return out;
	}
	// TODO: template this
	if (typeSrc == INT) {
		if (typeDst == BYTE) {
			int i = *((int*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 1);
			TupulByte l = (TupulByte) i;
			out[0] = l;
			return out;
		} else if (typeDst == SHORT) {
			int i = *((int*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 2);
			short l = (short) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			return out;
		} else if (typeDst == LONG) {
			int i = *((int*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 8);
			long long l = (long long) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			out[2] = bytes[2];
			out[3] = bytes[3];
			out[4] = bytes[4];
			out[5] = bytes[5];
			out[6] = bytes[6];
			out[7] = bytes[7];
			return out;
		} else if (typeDst == DOUBLE) {
			int i = *((int*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 8);
			double l = (double) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			out[2] = bytes[2];
			out[3] = bytes[3];
			out[4] = bytes[4];
			out[5] = bytes[5];
			out[6] = bytes[6];
			out[7] = bytes[7];
			return out;
		} else if (typeDst == FLOAT) {
			int i = *((int*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 4);
			float l = (float) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			out[2] = bytes[2];
			out[3] = bytes[3];
			return out;
		}
	}
	if (typeSrc == INT) {
		if (typeDst == BYTE) {
			long long i = *((long long*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 1);
			TupulByte l = (TupulByte) i;
			out[0] = l;
			return out;
		} else if (typeDst == SHORT) {
			long long i = *((long long*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 2);
			short l = (short) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			return out;
		} else if (typeDst == INT) {
			long long i = *((long long*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 4);
			int l = (int) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			out[2] = bytes[2];
			out[3] = bytes[3];
			return out;
		} else if (typeDst == DOUBLE) {
			long long i = *((long long*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 8);
			double l = (double) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			out[2] = bytes[2];
			out[3] = bytes[3];
			out[4] = bytes[4];
			out[5] = bytes[5];
			out[6] = bytes[6];
			out[7] = bytes[7];
			return out;
		} else if (typeDst == FLOAT) {
			long long i = *((long long*) val);
			TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 4);
			float l = (float) i;
			TupulByte* bytes = (TupulByte*) &l;
			out[0] = bytes[0];
			out[1] = bytes[1];
			out[2] = bytes[2];
			out[3] = bytes[3];
			return out;
		}
	}
	// TODO: throw exception
	return {0};
}

TupulByte* preferredType(TupulByte* type0, TupulByte* type1) {
	if (type0 == DOUBLE || type1 == DOUBLE) return DOUBLE;
	if (type0 == FLOAT || type1 == FLOAT) return FLOAT; // warning with automatic cast from from long to float
	if (type0 == LONG || type1 == LONG) return LONG;
	if (type0 == INT || type1 == INT) return INT;
	if (type0 == HALF || type1 == HALF) return HALF;
	if (type0 == SHORT || type1 == SHORT) return SHORT;
	if (type0 == BYTE || type1 == BYTE) return BYTE;
	// undefined, determined by using methods and seeing what the result is
	return UNDEFINED;
}
