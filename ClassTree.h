#pragma once
#include <string>
#include <vector>
#include "MethodTree.h"
#include "TupulClass.h"
using namespace std;

#define byte unsigned char

struct ClassTree {
    public: char* name;
    public: vector<MethodTree> methods;
};

ClassTree createClassTree(string contents);
TupulClass* finishClass(ClassTree tree);
