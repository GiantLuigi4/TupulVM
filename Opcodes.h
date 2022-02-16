#pragma once

#define byte unsigned char

extern byte CLASS;
extern byte METHOD;
extern byte DESCRIPTOR;
extern byte END;
extern byte RETURN;

extern byte LOCAL;
extern byte LOADC;
extern byte SETL;
extern byte LOADL;
extern byte MATH;

extern byte PUSH;
extern byte POP;

bool should_end(byte block_cause, byte current_op, bool second_block);
bool should_start_second_block(byte block_cause, byte current_op);
bool isOpcode(byte op);
bool is_expanded_opcode(byte op);
