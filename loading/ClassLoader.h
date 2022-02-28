#pragma once
#include <vector>
#include "../TupulClass.h"
#include "../SourceList.h"
#include "ClassTree.h"

using namespace std;

struct ClassLoader {
	public: vector<TupulClass*> classes;
	public: vector<TupulClass*> treeModifiers;
	public: SourceList* sources;
};

TupulClass* getClass(ClassLoader* loader, char* name);
TupulClass* loadClass(ClassLoader* loader, char* name);
TupulClass* passTree(ClassLoader* loader, ClassTree* tree);
void freeLoader(ClassLoader* loader);
