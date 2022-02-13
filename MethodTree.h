#pragma once
#include <string>
#include <vector>
#include "Insn.h"
using namespace std;

#define byte unsigned char

struct MethodTree {
    public: string name;
    public: string descr;
    public: vector<Insn> insns;
};

MethodTree methodTreeFor(vector<byte> blockDescr, vector<byte> blockFunc);
// TODO: complete
