#include "Types.h"

byte* BYTE = (byte[]) {0, 0};
byte* SHORT = (byte[]) {0, 1};
byte* HALF = (byte[]) {0, 2};
byte* INT = (byte[]) {0, 3};
byte* FLOAT = (byte[]) {0, 4};
byte* LONG = (byte[]) {0, 5};
byte* DOUBLE = (byte[]) {0, 6};

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