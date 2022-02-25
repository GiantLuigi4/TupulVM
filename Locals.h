#include <vector>
#include "Definitions.h"
using namespace std;

struct Locals {
	public: vector<TupulByte*> locals;
	public: vector<TupulByte*> localTypes;
	public: vector<TupulByte*> stack;
	public: vector<TupulByte*> stackTypes;
	public: vector<int> pushPoints;
};

void popStack(Locals* locals);
void freeLocals(Locals* locals);
