#pragma once
#include <string>
#include "TupulMethod.h"

struct ClassLoader;

using namespace std;

class TupulClass {
	public: string name;
	public: ClassLoader* loader;
	public: vector<TupulMethod*> methods;
};
