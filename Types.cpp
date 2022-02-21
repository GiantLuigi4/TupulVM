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
    if (type[0] == 8) free(type);
}

byte* copyType(byte* type) {
    if (type[0] == 8) {
		short len = ((type[1] & 0xFF) << 8) |
            		((type[2] & 0xFF) << 0);
        byte* out = (byte*) calloc(sizeof(byte), len);
        for (int i = 0; i < len; i++) out[i] = type[i];
        return out;
    }
    return type;
}

short getTypeLength(byte* type) {
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
byte* tupCast(byte* val, byte* typeSrc, byte* typeDst) {
    // printf("%i\n", typeSrc[0]);
    // printf("%i\n", typeDst[0]);
    if (typeDst == BYTE) {
        byte* out = (byte*) calloc(sizeof(byte), 1);
        out[0] = val[0];
        return out;
    }
    if (typeSrc == LONG) {
        if (typeDst == INT) {
            long long i0 = (((long long) val[0] & 0xFF) << 56) |
                           (((long long) val[1] & 0xFF) << 48) |
                           (((long long) val[2] & 0xFF) << 40) |
                           (((long long) val[3] & 0xFF) << 32) |
                           (((long long) val[4] & 0xFF) << 24) |
                           (((long long) val[5] & 0xFF) << 16) |
                           (((long long) val[6] & 0xFF) <<  8) |
                           (((long long) val[7] & 0xFF) <<  0) ;
            int i = (int) i0;
            
            byte* bytes = (byte*) calloc(sizeof(byte), 4);
            bytes[0] = (i >> 24) & 0xFF;
            bytes[1] = (i >> 16) & 0xFF;
            bytes[2] = (i >> 8) & 0xFF;
            bytes[3] = i & 0xFF;
            return bytes;
        }
    }
    if (typeSrc == INT) {
        if (typeDst == BYTE) {
            byte* out = (byte*) calloc(sizeof(byte), 1);
            // out[0] = val[0] - val[2];
            // TODO: get off C++ casts
            return out;
        } else if (typeDst == LONG) {
            byte* out = (byte*) calloc(sizeof(byte), 8);
            long long i = (long long) ((val[0] & 0xFF) << 24) |
                                      ((val[1] & 0xFF) << 16) |
                                      ((val[2] & 0xFF) <<  8) |
                                      ((val[3] & 0xFF) <<  0) ;

            out[0] = (i >> (long long) 56) & 0xFF;
            out[1] = (i >> (long long) 48) & 0xFF;
            out[2] = (i >> (long long) 40) & 0xFF;
            out[3] = (i >> (long long) 32) & 0xFF;
            out[4] = (i >> 24) & 0xFF;
            out[5] = (i >> 16) & 0xFF;
            out[6] = (i >> 8) & 0xFF;
            out[7] = i & 0xFF;

            // out[2] = val[0];
            // out[3] = val[1];
            // out[6] = val[2];
            // out[7] = val[3];

            // printf("%i,", out[0]);
            // printf("%i,", out[1]);
            // printf("%i,", out[2]);
            // printf("%i,", out[3]);
            // printf("%i,", out[4]);
            // printf("%i,", out[5]);
            // printf("%i,", out[6]);
            // printf("%i\n", out[7]);

            return out;
        }
    }
    // TODO: throw exception
}

byte* tupSum(byte* num0, byte* type0, byte* num1, byte* type1, byte** type) {
    type[0] = preferredType(type0, type1);
    // TODO: non-primitive sum
    if (type0 != type[0]) num0 = tupCast(num0, type0, type[0]);
    if (type1 != type[0]) num1 = tupCast(num1, type1, type[0]);
    if (type[0] == BYTE) {
        byte* bytes = (byte*) calloc(sizeof(byte), 1);
        bytes[0] = num0[0] + num1[0];
        return bytes;
    }
    if (type[0] == SHORT) {
        short i0 = ((num0[2] & 0xFF) <<  8) |
                   ((num0[3] & 0xFF) <<  0) ;
        
        short i1 = ((num1[2] & 0xFF) <<  8) |
                   ((num1[3] & 0xFF) <<  0) ;
        
        int i = i0 + i1;

        byte* bytes = (byte*) calloc(sizeof(byte), 4);
        bytes[0] = (i >> 24) & 0xFF;
        bytes[1] = (i >> 16) & 0xFF;
        bytes[2] = (i >> 8) & 0xFF;
        bytes[3] = i & 0xFF;
        return bytes;
    }
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
    if (type[0] == LONG) {
        long long i0 = (((long long) num0[0] & 0xFF) << 56) |
                       (((long long) num0[1] & 0xFF) << 48) |
                       (((long long) num0[2] & 0xFF) << 40) |
                       (((long long) num0[3] & 0xFF) << 32) |
                       (((long long) num0[4] & 0xFF) << 24) |
                       (((long long) num0[5] & 0xFF) << 16) |
                       (((long long) num0[6] & 0xFF) <<  8) |
                       (((long long) num0[7] & 0xFF) <<  0) ;
        
        long long i1 = (((long long) num1[0] & 0xFF) << 56) |
                       (((long long) num1[1] & 0xFF) << 48) |
                       (((long long) num1[2] & 0xFF) << 40) |
                       (((long long) num1[3] & 0xFF) << 32) |
                       (((long long) num1[4] & 0xFF) << 24) |
                       (((long long) num1[5] & 0xFF) << 16) |
                       (((long long) num1[6] & 0xFF) <<  8) |
                       (((long long) num1[7] & 0xFF) <<  0) ;
        
        long long i = i0 + i1;

        byte* bytes = (byte*) calloc(sizeof(byte), 8);
        bytes[0] = (i >> (long) 56) & 0xFF;
        bytes[1] = (i >> (long) 48) & 0xFF;
        bytes[2] = (i >> (long) 40) & 0xFF;
        bytes[3] = (i >> (long) 32) & 0xFF;
        bytes[4] = (i >> 24) & 0xFF;
        bytes[5] = (i >> 16) & 0xFF;
        bytes[6] = (i >> 8) & 0xFF;
        bytes[7] = i & 0xFF;
        return bytes;
    }
    return {0};
}

byte* tupDiv(byte* num0, byte* type0, byte* num1, byte* type1, byte** type) {
    type[0] = preferredType(type0, type1);
    // TODO: non-primitive sum
    if (type0 != type[0]) num0 = tupCast(num0, type0, type[0]);
    if (type1 != type[0]) num1 = tupCast(num1, type1, type[0]);

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
        bytes[2] = (i >>  8) & 0xFF;
        bytes[3] = (i      ) & 0xFF;
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
