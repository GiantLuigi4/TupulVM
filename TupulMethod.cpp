#include "TupulMethod.h"

/* begin interpreted methods */

// it's disgusting, but it works
void InterpretedMethod::setupMethod(vector<Insn> insns) {
    Insn* insnsPtr = (Insn*) calloc(sizeof(Insn), insns.size()); // create a pointer which will be used as a list of insns
    for (int i = 0; i < insns.size(); i++) insnsPtr[i] = insns[i]; // copy the vector's contents to the pointer
    Insn** insnsPtrPtr = (Insn**) calloc(sizeof(Insn*), 2); // create a list of two "Insn*"s, only one of which will be an Insn*
    insnsPtrPtr[0] = insnsPtr; // store insn list
    insnsPtrPtr[1] = (Insn*) (long) insns.size(); // store length
    this->context = (long) insnsPtrPtr; // set the context to this cursed nonsense

    this->run = execInterp; // run method
    this->free = freeInterpMethod; // run method
    // yeah idk, C++ is weird :wheeze:
}

void InterpretedMethod::set(string name, string descr) {
    this->name = name;
    this->descr = descr;
}

byte* execInterp(TupulMethod* method) {
    Insn** insns = (Insn**) method->context;
    long len = (long) insns[1];
    for (int i = 0; i < len; i++) {
        Insn insn = insns[0][i];
        printf("%i\n", insn.op);
    }
    return {0};
}

void freeInterpMethod(TupulMethod* method) {
    Insn** insns = (Insn**) method->context;
    long len = (long) insns[1];
    for (int i = 0; i < len; i++) {
        Insn insn = insns[0][i];
        Insns::freeInsn(insn);
    }
    free(insns[0]);
    // this crashes
    // free(insns[1]);
    free(insns);
    method->context = 0;
    method->name = "";
    method->descr = "";
}

/* end interpreted methods */