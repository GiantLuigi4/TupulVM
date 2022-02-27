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
	#if cachedPointers == 1 || useQalloc == 1
		void* trackedAlloc(size_t typeSize, size_t amount);
		void trackedFree(void* obj);
	#else
		#define trackedAlloc calloc
		#define trackedFree free
	#endif
#endif

void resultAllocs();
