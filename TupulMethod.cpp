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
TupulByte** execInterp(TupulMethod* method, Locals* locals) {
	Insn** insns = (Insn**) method->context;
	long len = (long) insns[1];
	for (int i = 0; i < len; i++) {
		Insn insn = insns[0][i];

		TupulByte x = insn.op;
		switch (x) {
			case 252: // LOCAL
				locals->locals.push_back((TupulByte*)0);
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
					// TODO: non-primitve
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
					TupulByte* bytes = (TupulByte*) trackedCalloc(sizeof(TupulByte), 4);
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
								TupulByte* bytes = (TupulByte*) trackedCalloc(sizeof(TupulByte), 4);
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
								TupulByte* bytes = (TupulByte*) trackedCalloc(sizeof(TupulByte), 8);
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
			case 248: { // SETL
					int i1 = locals->stack.size() - 1;
					int i2 = stoi(insn.arg0);
					TupulByte* stackElement = locals->stack[i1];
					TupulByte* stackType = locals->stackTypes[i1];
					TupulByte* out = tupCast(stackElement, stackType, locals->localTypes[i2]);
					trackedFree(stackElement);
					freeType(stackType);
					// freeType(locals->stackTypes[i1]);

					locals->stack.pop_back();
					locals->stackTypes.pop_back();
					// locals.stack.push_back(out);
					// locals.stackTypes.push_back(locals.localTypes[i2]);
					TupulByte* oldObj = locals->locals[i2];
					if (oldObj != nullptr) trackedFree(oldObj);
					locals->locals[i2] = out;
				}
				break;
			case 242: { // LOADL
					int i = stoi(insn.arg0);
					// locals->stack.push_back(locals->locals[i]);
					// locals->stackTypes.push_back(locals->localTypes[i]);

					TupulByte* bytes = locals->locals[i];
					TupulByte* bytesCopy = (TupulByte*) trackedCalloc(sizeof(TupulByte), len);
					for (int i = 0; i < len; i++) bytesCopy[i] = bytes[i];
					locals->stack.push_back(bytesCopy);
					locals->stackTypes.push_back(copyType(locals->localTypes[i]));
				}
				break;
			case 243: { // RETURN
					TupulByte** output = (TupulByte**) trackedCalloc(sizeof(TupulByte*), 2);
					TupulByte* bytes = locals->stack[locals->stack.size() - 1];
					TupulByte* type = locals->stackTypes[locals->stackTypes.size() - 1];

					TupulByte* typeCopy = copyType(type);
					int len = getTypeLength(typeCopy);
					TupulByte* bytesCopy = (TupulByte*) trackedCalloc(sizeof(TupulByte), len);
					for (int i = 0; i < len; i++) 
						bytesCopy[i] = bytes[i];

					output[0] = typeCopy;
					output[1] = bytesCopy;

					// free(bytes);
					// freeType(type);

					// bool contains = false;
					// for (byte* lbytes : locals->locals) {
					// 	if ((long long) lbytes == (long long) bytes) {
					// 		contains = true;
					// 		break;
					// 	}
					// }
					// if (!contains) free(bytes);

					// contains = false;
					// for (byte* ltype : locals->localTypes) {
					// 	if ((long long) ltype == (long long) type) {
					// 		contains = true;
					// 		break;
					// 	}
					// }
					// if (!contains) freeType(type);

					// locals->stack.pop_back();
					// locals->stackTypes.pop_back();
					freeLocals(locals);
					return output;
				}
				break;
			case 241: { // math
					int stackSize = locals->stack.size() - 1;
					TupulByte* bytes0 = locals->stack[stackSize];
					TupulByte* type0 = locals->stackTypes[stackSize];
					stackSize--;
					TupulByte* bytes1 = locals->stack[stackSize];
					TupulByte* type1 = locals->stackTypes[stackSize];
					locals->stack.pop_back();
					locals->stack.pop_back();
					locals->stackTypes.pop_back();
					locals->stackTypes.pop_back();
					// printf("%s\n", insn.arg0);

					TupulByte** typeOut = (TupulByte**) trackedCalloc(sizeof(TupulByte*), 1);
					TupulByte* result;
					// printf("%s\n%s\n", insn.arg0, insn.arg1);
					switch (insn.arg0[0]) {
						case '+':
							result = tupSum(bytes0, type0, bytes1, type1, typeOut);
							break;
						default:
							// TODO: print line number
							printf("ERROR: unimplemented math operator: %s\n", insn.arg0);
							TupulByte** vm_er = (TupulByte**) calloc(sizeof(TupulByte*), 2);
							TupulByte* type = (TupulByte*) calloc(sizeof(TupulByte), 1);
							type[0] = 254;
							TupulByte* result = (TupulByte*) calloc(sizeof(TupulByte), 2);
							result[0] = 1; // 1 byte
							result[1] = 0; // unimplemented operator
							vm_er[0] = type;
							vm_er[1] = result;
							return vm_er;
					}
					trackedFree(bytes0);
					trackedFree(bytes1);
					freeType(type0);
					freeType(type1);

					locals->stack.push_back(result);
					locals->stackTypes.push_back(typeOut[0]);
					trackedFree(typeOut);
				}
				break;
			case 240: { // ISTATIC
					TupulClass* methodOwner = method->owner;
					string arg0Str = insn.arg0;
					if (arg0Str.find_first_of("$") != -1) {
						string str = arg0Str.substr(0, arg0Str.find_first_of("$"));
						arg0Str = arg0Str.substr(arg0Str.find_first_of("$") + 1);
						// printf("%s\n", str.c_str());
						char* ownerCStr = (char*) str.c_str();
						methodOwner = getClass(method->owner->loader, ownerCStr);
					}
					TupulMethod* toInvoke = getMethod(methodOwner, (char*) arg0Str.c_str(), insn.arg1);

					Locals* l = (Locals*) trackedCalloc(sizeof(Locals), 1);
					// l->locals = locals->stack;
					// l->localTypes = locals->stackTypes;
					for (int i = 0; i < locals->stackTypes.size(); i++) {
						TupulByte* type = locals->stackTypes[i];
						TupulByte* bytes = locals->stack[i];
						int len = getTypeLength(type);
						TupulByte* bytesCopy = (TupulByte*) trackedCalloc(sizeof(TupulByte), len);
						for (int i = 0; i < len; i++) 
							bytesCopy[i] = bytes[i];
						
						freeType(type);
						trackedFree(bytes);

						l->localTypes.push_back(copyType(type));
						l->locals.push_back(bytesCopy);
					}
					locals->stack.clear();
					locals->stackTypes.clear();
					TupulByte** result = toInvoke->run(toInvoke, l);
					// for (int i = 0; i < locals->stackTypes.size(); i++) {
					// 	byte* type = locals->stackTypes[i];
					// 	byte* bytes = locals->stack[i];
					// 	bool containsType = false;
					// 	for (byte* ltype : locals->localTypes) {
					// 		if ((long long) ltype == (long long) type) {
					// 			containsType = true;
					// 			break;
					// 		}
					// 	}
					// 	if (!containsType) freeType(type);
					// 	containsType = false;
					// 	for (byte* lbytes : locals->locals) {
					// 		if ((long long) bytes == (long long) lbytes) {
					// 			containsType = true;
					// 			break;
					// 		}
					// 	}
					// 	if (!containsType) free(bytes);
					// }

					if (result[0][0] == 254) {
						return result; // allow the error to quickly end the program
					} else if (result[0][0] == 255) {
						// really should only be methods which are implemented in code, so no need for tracking it
						free(result[0]);
						free(result);
					} else {
						locals->stack.push_back(result[1]);
						locals->stackTypes.push_back(result[0]);
						trackedFree(result); // frees the pointer to the pointers, does not free the pointers themselves
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