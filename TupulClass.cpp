#include "TupulClass.h"

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
