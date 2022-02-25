#pragma once
#include <string>
#include <vector>
#include "Insn.h"
#include "TupulMethod.h"
#include <memory>
using namespace std;

#define byte unsigned char

struct TupulClass;

struct MethodTree {
	public: string name;
	public: string descr;
	public: vector<Insn> insns;
};

MethodTree methodTreeFor(vector<TupulByte> blockDescr, vector<TupulByte> blockFunc);
TupulMethod* finishMethod(MethodTree tree, TupulClass* clazz);
// TODO: complete
