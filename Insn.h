#pragma once
#include <string>
using namespace std;
#define byte unsigned char

struct Insn {
    public: byte op;
    public: char* arg0;
    public: char* arg1;
};

namespace Insns {
    void freeInsn(Insn insn);
};
