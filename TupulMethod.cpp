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

#include "Utils.h"
#include "Locals.h"
#include "Opcodes.h"
#include "Types.h"
byte** execInterp(TupulMethod* method) {
    Insn** insns = (Insn**) method->context;
    long len = (long) insns[1];
    Locals locals;
    for (int i = 0; i < len; i++) {
        Insn insn = insns[0][i];

        byte x = insn.op;
        //      if (x == 253){printf("descriptor ")    ;}
        // else if (x == 252){printf("local ")         ;}
        // else if (x == 251){printf("push ")          ;}
        // else if (x == 250){printf("pop ")           ;}
        // else if (x == 249){printf("loadc ")         ;}
        // else if (x == 248){printf("setl ")          ;}
        // else if (x == 243){printf("return ")        ;}
        // else if (x == 242){printf("loadl ")         ;}
        // else              {printf("%i ", x)         ;}
        // if (insn.arg0 != nullptr) printf("%s ", insn.arg0);
        // if (insn.arg1 != nullptr) printf("%s ", insn.arg1);
        // printf("\n");
        switch (x) {
            case 252: // LOCAL
                locals.locals.push_back((byte*)0);
                break;
            case 251: // PUSH
                locals.pushPoints.push_back(locals.stack.size());
                break;
            case 250: // POP
                popStack(locals);
                break;
            case 249: // LOADC
                if (insn.arg1 == nullptr) {
                    // https://stackoverflow.com/a/4442669
                    int i = stoi(insn.arg0);
                    byte* bytes = (byte*) calloc(sizeof(byte), 4);
                    // https://stackoverflow.com/a/43515755
                    bytes[0] = (i >> 24) & 0xFF;
                    bytes[1] = (i >> 16) & 0xFF;
                    bytes[2] = (i >> 8) & 0xFF;
                    bytes[3] = i & 0xFF;
                    locals.stack.push_back(bytes);
                    locals.stackTypes.push_back(INT);
                } else {
                    switch (insn.arg1[0]) {
                        case 'I':
                            // https://stackoverflow.com/a/4442669
                            int i = stoi(insn.arg0);
                            byte* bytes = (byte*) calloc(sizeof(byte), 4);
                            // https://stackoverflow.com/a/43515755
                            bytes[0] = (i >> 24) & 0xFF;
                            bytes[1] = (i >> 16) & 0xFF;
                            bytes[2] = (i >> 8) & 0xFF;
                            bytes[3] = i & 0xFF;
                            locals.stack.push_back(bytes);
                            locals.stackTypes.push_back(INT);
                            break;
                    }
                }
                break;
            case 248: {// SETL
                    int i1 = locals.stack.size() - 1;
                    locals.locals[stoi(insn.arg0)] = locals.stack[i1];
                    locals.stack.pop_back();
                    locals.localTypes.push_back(locals.stackTypes[i1]);
                    locals.localTypes.pop_back();
                }
                break;
            case 242: {// LOADL
                    locals.stack.push_back(locals.locals[stoi(insn.arg0)]);
                    locals.stackTypes.push_back(locals.localTypes[stoi(insn.arg0)]);
                }
                break;
            case 243: { // RETURN
                    byte* bytes = locals.stack[locals.stack.size() - 1];
                    byte** output = (byte**) calloc(sizeof(byte*), 2);
                    byte* type = locals.stackTypes[locals.stackTypes.size() - 1];

                    byte* typeCopy = copyType(type);
                    short len = getTypeLength(typeCopy);
                    byte* bytesCopy = (byte*) calloc(sizeof(byte), len);
                    for (short i = 0; i < len; i++) bytesCopy[i] = bytes[i];

                    output[0] = typeCopy;
                    output[1] = bytesCopy;

                    locals.stack.pop_back();
                    locals.stackTypes.pop_back();
                    freeLocals(locals);
                    return output;
                }
                break;
        }
    }
    freeLocals(locals);
    return nullptr;
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