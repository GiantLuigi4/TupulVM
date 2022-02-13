#include "SourceList.h"

#include <cstdio>

byte* SourceList::getContents(string name) {
	return this->sources[name];
}
