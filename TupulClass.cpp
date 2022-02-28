#include "TupulClass.h"
#include "Utils.h"
#include <string.h>

// TODO: sort methods by name for sake of binary search
TupulMethod* getMethod(TupulClass* clazz, char* name, char* descr) {
	string nameStr = name;
	string descrStr = descr;
	for (TupulMethod* method : clazz->methods) {
		if (method->name == nameStr) {
			string desc = method->descr;
			desc = desc.substr(desc.find_first_of("("));
			if (desc == descrStr) {
				return method;
			}
		}
	}
	return nullptr;
}

// TODO: sort fields by name for sake of binary search
TupulField* getField(TupulClass* clazz, char* name) {
	TupulField** fieldsOffset = clazz->fields + 1;
	for (unsigned long long l = 0; l < (unsigned long long) clazz->fields[0]; l++) {
		TupulField* field = fieldsOffset[l];
		char* fname = field->name;
		if (strcmp(fname, name) != 0) {
			return field;
		}
	}
}

void freeClass(TupulClass* clazz) {
	#ifdef MEM_TRACK_COUNT
		int allocs = tallyAllocs();
		printf("Freeing class: \"%s\", starting with %i pointers\n", clazz->name, allocs);
	#endif
	for (TupulMethod* method : clazz->methods) method->free(method);
	for (unsigned long long l = 0; l < (unsigned long long) clazz->fields[0]; l++) freeField(clazz->fields[l + 1]);
	free(clazz->fields);
	trackedFree(clazz->fieldBytes);
	trackedFree(clazz->name);
	trackedFree(clazz);
	#ifdef MEM_TRACK_COUNT
		int finalAllocs = tallyAllocs();
		printf("Freeing class, allocated %i pointers\n", finalAllocs - allocs);
	#endif
}
