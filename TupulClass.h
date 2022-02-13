#pragma once
#include <string>

struct ClassLoader;

using namespace std;

class TupulClass {
	public: string name;
	public: ClassLoader* loader;
};
