#pragma once
#include <string>
#include <vector>
#include "Insn.h"
#include <memory>
using namespace std;

// TODO: figure out some way to automate binding native methods to tupul methods :wheeze:
class TupulMethod {
    public: string name = 0;
    public: string descr = 0;
    public: long context;
    public: byte* (*run)(TupulMethod*); // TODO: args
    public: void (*free)(TupulMethod*); // TODO: args
    public: virtual void set(string name, string descr) = 0;
    public: virtual ~TupulMethod() = default;
};

// class CompiledMethod : TupulMethod {
//     // TODO: learn how to do this
// };

class InterpretedMethod : public TupulMethod {
    // called after compiliation or when the owning class gets offloaded
    // once a method is compiled, it no longer needs to be interpreted, and thus the instructions can be freed
    public: void freeMethod();
    public: void setupMethod(vector<Insn> insns);
    public: void set(string name, string descr);
};

byte* execInterp(TupulMethod* method);
void freeInterpMethod(TupulMethod* method);
