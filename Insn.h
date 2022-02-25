#pragma once
#include <string>
using namespace std;
#include "Definitions.h"

struct Insn {
	public: TupulByte op;
	public: char* arg0;
	public: char* arg1;
};

namespace Insns {
	void freeInsn(Insn insn);
};
