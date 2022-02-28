#include "TupulField.h"
#include "../Utils.h"
#include <string.h>

void freeField(TupulField* field) {
	trackedFree(field->name);
	trackedFree(field);
}

TupulByte* getStatic(TupulField* field) {
	if (!field->isStatic) {
		// TODO: throw a tupul exception
		throw nullptr;
	}
	TupulClass* owner = field->owner;
	TupulByte* bytes = owner->fieldBytes;
	unsigned long long off = field->offset;
	TupulByte* type = field->type;
	unsigned short len = getTypeLength(type);
	TupulByte* val = (TupulByte*) trackedAlloc(sizeof(TupulByte), len);
	TupulByte* fields = owner->fieldBytes;
	// for (unsigned short s = 0; s < len; s++) val[s] = fields[off + s];
	memccpy(val, fields, off, len);
}

TupulField* makeField(unsigned long long offset, bool stat, string name, TupulByte* type, TupulClass* owner/*, int bitOffset*/) {
	TupulField* field = (TupulField*) trackedAlloc(sizeof(TupulField), 1);
	char* namec = (char*) trackedAlloc(sizeof(char), name.length());
	memcpy(namec, name.c_str(), name.length());
	field->name = namec;
	field->isStatic = stat;
	field->offset = offset;
	field->type = type;
	field->owner = owner;
}