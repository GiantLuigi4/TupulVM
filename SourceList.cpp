#include "SourceList.h"

#include <cstdio>

char* SourceList::getContents(char* name) {
	return this->sources[name];
}

bool SourceList::containsName(char* name) {
	return this->sources.count(name) != 0;
}
