#include "TupulMethod.h"

/* begin interpreted methods */

// it's disgusting, but it works
void setupInterpretedMethod(TupulMethod* method, vector<Insn> insns, TupulClass* clazz) {
	Insn* insnsPtr = (Insn*) calloc(sizeof(Insn), insns.size()); // create a pointer which will be used as a list of insns
	for (int i = 0; i < insns.size(); i++) insnsPtr[i] = insns[i]; // copy the vector's contents to the pointer
	Insn** insnsPtrPtr = (Insn**) calloc(sizeof(Insn*), 2); // create a list of two "Insn*"s, only one of which will be an Insn*
	insnsPtrPtr[0] = insnsPtr; // store insn list
	insnsPtrPtr[1] = (Insn*) (long) insns.size(); // store length
	method->context = (long) insnsPtrPtr; // set the context to this cursed nonsense

	method->run = execInterp; // run method
	method->free = freeInterpMethod; // run method
	method->owner = clazz;
	// yeah idk, C++ is weird :wheeze:
}

#include "Utils.h"
#include "Locals.h"
#include "Opcodes.h"
#include "Types.h"
#include "TupulClass.h"
#include "ClassLoader.h"
byte** execInterp(TupulMethod* method, Locals* locals) {
	Insn** insns = (Insn**) method->context;
	long len = (long) insns[1];
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
				locals->locals.push_back((byte*)0);
				switch (insn.arg1[0]) {
					case 'I':
						locals->localTypes.push_back(INT);
						break;
					case 'L':
						locals->localTypes.push_back(LONG);
						break;
					case 'D':
						locals->localTypes.push_back(DOUBLE);
						break;
					case 'H':
						locals->localTypes.push_back(HALF);
						break;
					case 'F':
						locals->localTypes.push_back(FLOAT);
						break;
					case 'C':
						locals->localTypes.push_back(CHAR);
						break;
					case 'S':
						locals->localTypes.push_back(SHORT);
						break;
					// TODO: byte, boolean
				}
				break;
			case 251: // PUSH
				locals->pushPoints.push_back(locals->stack.size());
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
					locals->stack.push_back(bytes);
					locals->stackTypes.push_back(INT);
				} else {
					switch (insn.arg1[0]) {
						case 'I': {
								// https://stackoverflow.com/a/4442669
								int i = stoi(insn.arg0);
								byte* bytes = (byte*) calloc(sizeof(byte), 4);
								// https://stackoverflow.com/a/43515755
								bytes[0] = (i >> 24) & 0xFF;
								bytes[1] = (i >> 16) & 0xFF;
								bytes[2] = (i >> 8) & 0xFF;
								bytes[3] = i & 0xFF;
								locals->stack.push_back(bytes);
								locals->stackTypes.push_back(INT);
							}
							break;
						case 'L': {
								// https://stackoverflow.com/a/4442669
								long long i = stol(insn.arg0);
								byte* bytes = (byte*) calloc(sizeof(byte), 8);
								// https://stackoverflow.com/a/43515755
								bytes[0] = (i >> (long) 56) & 0xFF;
								bytes[1] = (i >> (long) 48) & 0xFF;
								bytes[2] = (i >> (long) 40) & 0xFF;
								bytes[3] = (i >> (long) 32) & 0xFF;
								bytes[4] = (i >> 24) & 0xFF;
								bytes[5] = (i >> 16) & 0xFF;
								bytes[6] = (i >> 8) & 0xFF;
								bytes[7] = i & 0xFF;
								locals->stack.push_back(bytes);
								locals->stackTypes.push_back(LONG);
							}
							break;
					}
				}
				break;
			case 248: {// SETL
				// TODO: make stuff not get lost from memory
					int i1 = locals->stack.size() - 1;
					int i2 = stoi(insn.arg0);
					byte* stackElement = locals->stack[i1];
					// int i0 = ((stackElement[0] & 0xFF) << 24) |
					//     ((stackElement[1] & 0xFF) << 16) |
					//     ((stackElement[2] & 0xFF) <<  8) |
					//     ((stackElement[3] & 0xFF) <<  0) ;
					// printf("stack type = %i\n", locals.stackTypes[i1]);
					// printf("stack element = %i\n", i0);
					byte* out = tupCast(stackElement, locals->stackTypes[i1], locals->localTypes[i2]);
					// i0 = ((out[0] & 0xFF) << 24) |
					//     ((out[1] & 0xFF) << 16) |
					//     ((out[2] & 0xFF) <<  8) |
					//     ((out[3] & 0xFF) <<  0) ;
					free(stackElement);
					freeType(locals->stackTypes[i1]);

					locals->stack.pop_back();
					locals->stackTypes.pop_back();
					// locals.stack.push_back(out);
					// locals.stackTypes.push_back(locals.localTypes[i2]);
					locals->locals[i2] = out;
				}
				break;
			case 242: {// LOADL
					int i = stoi(insn.arg0);
					locals->stack.push_back(locals->locals[i]);
					locals->stackTypes.push_back(locals->localTypes[i]);
				}
				break;
			case 243: { // RETURN
					byte** output = (byte**) calloc(sizeof(byte*), 2);
					byte* bytes = locals->stack[locals->stack.size() - 1];
					byte* type = locals->stackTypes[locals->stackTypes.size() - 1];

					byte* typeCopy = copyType(type);
					int len = getTypeLength(typeCopy);
					byte* bytesCopy = (byte*) calloc(sizeof(byte), len);
					for (int i = 0; i < len; i++) 
						bytesCopy[i] = bytes[i];

					output[0] = typeCopy;
					output[1] = bytesCopy;

					locals->stack.pop_back();
					locals->stackTypes.pop_back();
					freeLocals(locals);
					return output;
				}
				break;
			case 241: { // math
					int stackSize = locals->stack.size() - 1;
					byte* bytes0 = locals->stack[stackSize];
					byte* type0 = locals->stackTypes[stackSize];
					stackSize--;
					byte* bytes1 = locals->stack[stackSize];
					byte* type1 = locals->stackTypes[stackSize];
					locals->stack.pop_back();
					locals->stack.pop_back();
					locals->stackTypes.pop_back();
					locals->stackTypes.pop_back();
					// printf("%s\n", insn.arg0);

					byte** typeOut = (byte**) calloc(sizeof(byte*), 1);
					byte* result;
					// printf("%s\n%s\n", insn.arg0, insn.arg1);
					switch (insn.arg0[0]) {
						case '+':
							result = tupSum(bytes0, type0, bytes1, type1, typeOut);
							break;
						default:
							// TODO: print line number
							printf("ERROR: unimplemented math operator: %s\n", insn.arg0);
							byte** vm_er = (byte**) calloc(sizeof(byte*), 2);
							byte* type = (byte*) calloc(sizeof(byte), 1);
							type[0] = 254;
							byte* result = (byte*) calloc(sizeof(byte), 2);
							result[0] = 1; // 1 byte
							result[1] = 0; // unimplemented operator
							vm_er[0] = type;
							vm_er[1] = result;
							return vm_er;
					}
					// long long i0 = (((long long) result[0] & 0xFF) << 56) |
					//    (((long long) result[1] & 0xFF) << 48) |
					//    (((long long) result[2] & 0xFF) << 40) |
					//    (((long long) result[3] & 0xFF) << 32) |
					//    (((long long) result[4] & 0xFF) << 24) |
					//    (((long long) result[5] & 0xFF) << 16) |
					//    (((long long) result[6] & 0xFF) <<  8) |
					//    (((long long) result[7] & 0xFF) <<  0) ;
					// printf("result = %i\n", i0);
					free(bytes0);
					free(bytes1);
					freeType(type0);
					freeType(type1);
					// i0 = (((long long) result[0] & 0xFF) << 56) |
					//    (((long long) result[1] & 0xFF) << 48) |
					//    (((long long) result[2] & 0xFF) << 40) |
					//    (((long long) result[3] & 0xFF) << 32) |
					//    (((long long) result[4] & 0xFF) << 24) |
					//    (((long long) result[5] & 0xFF) << 16) |
					//    (((long long) result[6] & 0xFF) <<  8) |
					//    (((long long) result[7] & 0xFF) <<  0) ;
					// printf("result = %i\n", i0);

					locals->stack.push_back(result);
					locals->stackTypes.push_back(typeOut[0]);
				}
				break;
			case 240: { // ISTATIC
					TupulClass* methodOwner = method->owner;
					string arg0Str = insn.arg0;
					if (arg0Str.find_first_of("$") != -1) {
						string str = arg0Str.substr(0, arg0Str.find_first_of("$"));
						arg0Str = arg0Str.substr(arg0Str.find_first_of("$") + 1);
						// printf("%s\n", str.c_str());
						methodOwner = getClass(method->owner->loader, (char*) str.c_str());
					}
					TupulMethod* toInvoke = getMethod(methodOwner, (char*) arg0Str.c_str(), insn.arg1);

					Locals* l = (Locals*) calloc(sizeof(Locals), 1);
					// l->locals = locals->stack;
					// l->localTypes = locals->stackTypes;
					for (int i = 0; i < locals->stackTypes.size(); i++) {
						byte* type = locals->stackTypes[i];
						byte* bytes = locals->stack[i];
						l->localTypes.push_back(copyType(type));
						int len = getTypeLength(type);
						byte* bytesCopy = (byte*) calloc(sizeof(byte), len);
						for (int i = 0; i < len; i++) 
							bytesCopy[i] = bytes[i];
						
						l->locals.push_back(bytesCopy);
					}
					// printf("%s\n", toInvoke->name.c_str());
					// printf("%s\n", toInvoke->descr.c_str());
					// printf("%i\n", toInvoke->run);
					byte** result = toInvoke->run(toInvoke, l);
					for (int i = 0; i < locals->stackTypes.size(); i++) {
						byte* type = locals->stackTypes[i];
						byte* bytes = locals->stack[i];
						bool containsType = false;
						for (byte* ltype : locals->localTypes) {
							if ((long long) ltype == (long long) type) {
								containsType = true;
								break;
							}
						}
						if (!containsType) freeType(type);
						containsType = false;
						for (byte* lbytes : locals->locals) {
							if ((long long) bytes == (long long) lbytes) {
								containsType = true;
								break;
							}
						}
						if (!containsType) free(bytes);
					}
					locals->stack.clear();
					locals->stackTypes.clear();

					if (result[0][0] == 254) {
						return result; // allow the error to quickly end the program
					} else if (result[0][0] == 255) {
						free(result[0]);
						free(result);
					} else {
						locals->stack.push_back(result[1]);
						locals->stackTypes.push_back(result[0]);
						free(result); // frees the pointer to the pointers, does not free the pointers themselves
					}
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