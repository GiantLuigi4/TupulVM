#pragma once
#include <string>
#include "TupulMethod.h"
#include "data/TupulField.h"

struct ClassLoader;

using namespace std;

struct TupulClass {
	public: char* name;
	public: ClassLoader* loader;
	// TODO: switch to pointer pointer
	public: vector<TupulMethod*> methods;
	public: TupulField** fields;
	public: TupulByte* fieldBytes;
};

TupulMethod* getMethod(TupulClass* clazz, char* name, char* descr);
void freeClass(TupulClass* clazz);
