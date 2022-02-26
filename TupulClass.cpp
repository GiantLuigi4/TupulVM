#include "TupulClass.h"
#include "Utils.h"

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

void freeClass(TupulClass* clazz) {
	#ifdef MEM_TRACK_COUNT
		int allocs = tallyAllocs();
		printf("Freeing class: \"%s\", starting with %i pointers\n", clazz->name, allocs);
	#endif
	for (TupulMethod* method : clazz->methods) method->free(method);
	trackedFree(clazz->name);
	trackedFree(clazz);
	#ifdef MEM_TRACK_COUNT
		int finalAllocs = tallyAllocs();
		printf("Freeing class, allocated %i pointers\n", finalAllocs - allocs);
	#endif
}
