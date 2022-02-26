#pragma once
#include <string>
#include "Definitions.h"

using namespace std;

void print(string str);
void println(string str);
char* readFile(string fileName);
long long getTimeForPerformance();
bool startsWith(string str, string prefix);
int tallyAllocs();

#ifdef MEM_TRACK
	void* trackedAlloc(size_t typeSize, size_t amount);
	void trackedFree(void* obj);
#else
	#define trackedCalloc calloc
	#define trackedFree free
#endif

void resultAllocs();
