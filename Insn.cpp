#include "Insn.h"

#include "Utils.h"

void Insns::freeInsn(Insn insn) {
	if (insn.arg0 != null) trackedFree(insn.arg0);
	if (insn.arg1 != null) trackedFree(insn.arg1);
}
