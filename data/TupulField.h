#pragma once

#include "Types.h"
#include "../TupulClass.h"

struct TupulField {
	public: char* name;
	public: unsigned long long offset;
	public: bool isStatic;
	public: TupulByte* type;
	public: TupulClass* owner;
};

TupulField* makeField(unsigned long long offset, bool stat, string name, TupulByte* type, TupulClass* owner/*, int bitOffset*/);
TupulByte* getStatic(TupulField* field);
void freeField(TupulField* field);
