#include <vector>
using namespace std;
#define byte unsigned char

struct Locals {
	public: vector<byte*> locals;
	public: vector<byte*> localTypes;
	public: vector<byte*> stack;
	public: vector<byte*> stackTypes;
	public: vector<int> pushPoints;
};

void popStack(Locals* locals);
void freeLocals(Locals* locals);
