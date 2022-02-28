#include "header.h"

void freeSTDMethod(TupulMethod* method) {
	// nothing to do
}

TupulMethod* makeSTDMethod(TupulClass* clazz, string name, string descr, TupulByte** (*exec)(TupulMethod*,Locals*)) {
	// this just be how native methods be, yk
	TupulMethod* method = (TupulMethod*) trackedAlloc(sizeof(TupulMethod), 1);
	method->name = name;
	method->descr = descr;
	method->run = exec;
	method->free = freeSTDMethod;
	return method;
}