#pragma once
#include <string>
#include <vector>
#include "Insn.h"
#include <memory>
using namespace std;

// TODO: figure out some way to automate binding native methods to tupul methods :wheeze:
struct TupulMethod {
    public: string name = "";
    public: string descr = "";
    public: long context = 0;
    public: byte** (*run)(TupulMethod*); // TODO: args
    public: void (*free)(TupulMethod*);
};

// class CompiledMethod : TupulMethod {
//     // TODO: learn how to do this
// };

void setupInterpretedMethod(TupulMethod* method, vector<Insn> insns);
byte** execInterp(TupulMethod* method);
void freeInterpMethod(TupulMethod* method);
