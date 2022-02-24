#include "Insn.h"

void Insns::freeInsn(Insn insn) {
	free(insn.arg0);
	free(insn.arg1);
}
