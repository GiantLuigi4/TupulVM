#include "Opcodes.h"

TupulByte CLASS = 255;
TupulByte METHOD = 254;
TupulByte DESCRIPTOR = 253;
TupulByte END = 246;
TupulByte RETURN = 243;

TupulByte LOCAL = 252;
TupulByte LOADC = 249;
TupulByte SETL = 248;
TupulByte LOADL = 242;
TupulByte MATH = 241;
TupulByte ISTATIC = 240;

TupulByte PUSH = 251;
TupulByte POP = 250;

bool should_end(TupulByte block_cause, TupulByte current_op, bool second_block) {
	if (block_cause == METHOD) {
		return second_block && current_op == END;
	}
	return current_op == END;
}

bool should_start_second_block(TupulByte block_cause, TupulByte current_op) {
	if (block_cause == METHOD) {
		return current_op == END;
	}
	return current_op == DESCRIPTOR;
}

bool isOpcode(TupulByte op) {
	return op > 200; // TODO: find the exact value
}

bool is_expanded_opcode(TupulByte op) {
	return
		op == METHOD  ||
		op == LOCAL   ||
		op == SETL    ||
		op == LOADC   ||
		op == MATH    ||
		op == LOADL   ||
		op == ISTATIC  ;
}
