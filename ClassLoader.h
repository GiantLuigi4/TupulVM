#pragma once
#include <vector>
#include "TupulClass.h"
#include "SourceList.h"

using namespace std;

class ClassLoader {
	public: vector<TupulClass*> classes = {};
	public: SourceList sources;
};
