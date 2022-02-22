#pragma once
#include <string>
#include "TupulMethod.h"

struct ClassLoader;

using namespace std;

struct TupulClass {
	public: char* name;
	public: ClassLoader* loader;
	public: vector<TupulMethod*> methods;
};

TupulMethod* getMethod(TupulClass* clazz, char* name, char* descr);
void freeClass(TupulClass* clazz);