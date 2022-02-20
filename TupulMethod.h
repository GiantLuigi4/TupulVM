#pragma once
#include <string>
#include <vector>
#include "Insn.h"
#include <memory>
using namespace std;

struct TupulClass;

// TODO: figure out some way to automate binding native methods to tupul methods :wheeze:
struct TupulMethod {
    public: string name = "";
    public: string descr = "";
    public: long context = 0;
    public: byte** (*run)(TupulMethod*); // TODO: args
    public: void (*free)(TupulMethod*);
    public: TupulClass* owner;
};

// class CompiledMethod : TupulMethod {
//     // TODO: learn how to do this
// };

void setupInterpretedMethod(TupulMethod* method, vector<Insn> insns, TupulClass* clazz);
byte** execInterp(TupulMethod* method);
void freeInterpMethod(TupulMethod* method);
