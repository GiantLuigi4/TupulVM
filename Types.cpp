#include "Types.h"

byte* BYTE      =   (byte[]) {0  };
byte* SHORT     =   (byte[]) {1  };
byte* CHAR      =   (byte[]) {2  };
byte* HALF      =   (byte[]) {3  };
byte* INT       =   (byte[]) {4  };
byte* FLOAT     =   (byte[]) {5  };
byte* LONG      =   (byte[]) {6  };
byte* DOUBLE    =   (byte[]) {7  };
byte* UNDEFINED =   (byte[]) {255};

#include <stdlib.h>

void freeType(byte* type) {
    if (type[0] == 1) free(type);
}

byte* copyType(byte* type) {
    if (type[0] == 1) {
		short len = ((type[1] & 0xFF) << 8) |
            		((type[2] & 0xFF) << 0);
        byte* out = (byte*) calloc(sizeof(byte), len);
        for (int i = 0; i < len; i++) out[i] = type[i];
        return out;
    }
    return type;
}

short getTypeLength(byte* type) {
    if (type[0] == 0) {
        // well that's neat
        switch (type[1]) {
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

byte* tupSum(byte* num0, byte* type0, byte* num1, byte* type1, byte** type) {
    type[0] = preferredType(type0, type1);
    // TODO: non-primitive sum
    // TODO: casts
    if (type[0] == INT) {
        int i0 = ((num0[0] & 0xFF) << 24) |
                 ((num0[1] & 0xFF) << 16) |
                 ((num0[2] & 0xFF) <<  8) |
                 ((num0[3] & 0xFF) <<  0) ;
        
        int i1 = ((num1[0] & 0xFF) << 24) |
                 ((num1[1] & 0xFF) << 16) |
                 ((num1[2] & 0xFF) <<  8) |
                 ((num1[3] & 0xFF) <<  0) ;
        
        int i = i0 + i1;

        byte* bytes = (byte*) calloc(sizeof(byte), 4);
        bytes[0] = (i >> 24) & 0xFF;
        bytes[1] = (i >> 16) & 0xFF;
        bytes[2] = (i >> 8) & 0xFF;
        bytes[3] = i & 0xFF;
        return bytes;
    }
    return {0};
}

byte* tupDiv(byte* num0, byte* type0, byte* num1, byte* type1, byte** type) {
    type[0] = preferredType(type0, type1);
    // TODO: non-primitive sum
    // TODO: casts
    if (type[0] == INT) {
        int i0 = ((num0[0] & 0xFF) << 24) |
                 ((num0[1] & 0xFF) << 16) |
                 ((num0[2] & 0xFF) <<  8) |
                 ((num0[3] & 0xFF) <<  0) ;
        
        int i1 = ((num1[0] & 0xFF) << 24) |
                 ((num1[1] & 0xFF) << 16) |
                 ((num1[2] & 0xFF) <<  8) |
                 ((num1[3] & 0xFF) <<  0) ;
        
        int i = i0 / i1;

        byte* bytes = (byte*) calloc(sizeof(byte), 4);
        bytes[0] = (i >> 24) & 0xFF;
        bytes[1] = (i >> 16) & 0xFF;
        bytes[2] = (i >> 8) & 0xFF;
        bytes[3] = i & 0xFF;
        return bytes;
    }
    return {0};
}

byte* preferredType(byte* type0, byte* type1) {
    if (type0 == DOUBLE || type1 == DOUBLE) return DOUBLE;
    if (type0 == FLOAT || type1 == FLOAT) return FLOAT; // warning from long to float
    if (type0 == LONG || type1 == LONG) return LONG;
    if (type0 == INT || type1 == INT) return INT;
    if (type0 == HALF || type1 == HALF) return HALF;
    if (type0 == SHORT || type1 == SHORT) return SHORT;
    if (type0 == BYTE || type1 == BYTE) return BYTE;
    // undefined, determined by using methods and seeing what the result is
    return UNDEFINED;
}
