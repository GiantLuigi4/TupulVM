#pragma once
#include "Definitions.h"

extern TupulByte CLASS;
extern TupulByte METHOD;
extern TupulByte DESCRIPTOR;
extern TupulByte END;
extern TupulByte RETURN;

extern TupulByte LOCAL;
extern TupulByte LOADC;
extern TupulByte SETL;
extern TupulByte LOADL;
extern TupulByte MATH;

extern TupulByte PUSH;
extern TupulByte POP;

bool should_end(TupulByte block_cause, TupulByte current_op, bool second_block);
bool should_start_second_block(TupulByte block_cause, TupulByte current_op);
bool isOpcode(TupulByte op);
bool is_expanded_opcode(TupulByte op);
