#include "Opcodes.h"

byte CLASS = 255;
byte METHOD = 254;
byte DESCRIPTOR = 253;
byte END = 246;
byte RETURN = 243;

byte LOCAL = 252;
byte LOADC = 249;
byte SETL = 248;
byte LOADL = 242;

byte PUSH = 251;
byte POP = 250;

bool should_end(byte block_cause, byte current_op, bool second_block) {
    if (block_cause == METHOD) {
        return second_block && current_op == END;
    }
    return current_op == END;
}

bool should_start_second_block(byte block_cause, byte current_op) {
    if (block_cause == METHOD) {
        return current_op == END;
    }
    return current_op == DESCRIPTOR;
}

bool isOpcode(byte op) {
    return op > 200; // TODO: find the exact value
}

bool is_expanded_opcode(byte op) {
    return
        op == METHOD ||
        op == LOCAL ||
        op == SETL ||
        op == LOADC ||
        op == LOADL;
}