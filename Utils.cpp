#define _CRT_SECURE_NO_WARNINGS

#include "Utils.h"

#include <cstdio>
#include <string>

using namespace std;

#define null nullptr

// prints a c++ string
void print(string str) {
	printf("%s", str.c_str());
}

// prints a c++ string... but with a new line at the end!
void println(string str) {
	printf("%s\n", str.c_str());
}

// reads a file
#if defined(_WIN32) || defined(WIN32)
	#include <stringapiset.h>
#else
	#include <locale>
	#include <codecvt>
#endif
#include "FileSystems.h"
char* readFile(string name) {
	FILE* fp;
	// println(name);
	fp = fopen(absolutePath(name).c_str(), "r");
	if (fp == null) {
		printf("File not found: %s\n", absolutePath(name).c_str());
		// TODO: throw vm error
		return null;
	}
	fseek(fp, 0, SEEK_END);
	// https://stackoverflow.com/a/238607
	int len = ftell(fp);
	rewind(fp);
	char* contents = (char*) trackedAlloc(sizeof(char), len);
	fread(contents, sizeof(char), len, fp);

	fclose(fp); // fclose frees fp
	// bytecode spec: class name opcode must be the first character of the file
	// reason: actually makes it easier to check encoding, and thus also easier to deal with encoding accordingly
	// https://en.cppreference.com/w/cpp/locale/wstring_convert/from_bytes
	if (contents[0] == -61) {
		// thank you windows... for making me have to do this
		#if defined(_WIN32) || defined(WIN32)
			// https://codingtidbit.com/2020/02/09/c17-codecvt_utf8-is-deprecated/
			// https://stackoverflow.com/a/3999597
			string str = contents;
			int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
			std::wstring wstrTo(sizeNeeded, 0);
			MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], sizeNeeded);
			char* out = (char*) trackedAlloc(sizeof(char), sizeNeeded);
			for (int i = 0; i < sizeNeeded; i++) out[i] = wstrTo[i];
		#else
			wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> converter;
			u16string utf16 = converter.from_bytes(contents);
			char* out = (char*) trackedAlloc(sizeof(char), utf16.length());
			for (int i = 0; i < utf16.length(); i++) out[i] = utf16[i];
		#endif
		
		trackedFree(contents);
		return out;
	}

	return contents;
}

#include "Definitions.h"

#include <cstring>
#include <map>
#include <vector>

#ifdef MEM_TRACK
	int leftovers = 0;
#endif

#if cachedPointers == 1
	map<int, vector<void*>> freeBytes;

	int getLen(void* obj) {
		int i = 0;
		TupulByte* chr = (TupulByte*) trackedAlloc(sizeof(TupulByte), 1);
		TupulByte* objBytes = (TupulByte*) obj;
		for (;;) {
			i++;
			memcpy(chr, objBytes + i, 1);
			if (chr[0] == '\0') {
				#ifdef MEM_TRACK
					leftovers -= 1;
				#endif
	
				int len = 1;
				if (freeBytes.count(len) == 0) {
					vector<void*> blocks;
					freeBytes.insert(pair<int, vector<void*>>(len, blocks));
				}
				freeBytes.at(len).push_back(obj);

				return i;
			}
		}
		
		#ifdef MEM_TRACK
			leftovers -= 1;
		#endif
		int len = 1;
		if (freeBytes.count(len) == 0) {
			vector<void*> blocks;
			freeBytes.insert(pair<int, vector<void*>>(len, blocks));
		}
		freeBytes.at(len).push_back(obj);
		
		return -1;
	}
#endif

void* qalloc(size_t typeSize, size_t amount) {
	// return calloc(typeSize, amount);
	int sz = typeSize * amount;
	#if cachedPointers == 1
		if (freeBytes.count(sz) != 0) {
			vector<void*> vec = freeBytes.at(sz);
			void* vd = vec[vec.size() - 1];
			vec.pop_back();
			memset(vd, 0, sz + 1);
			return vd;
		}
	#endif
	void* result = (void*) malloc(sz + 1);
	// TODO: move qalloc to be it's own thing, this creates some finicky behavior with structs
	// memset(result + sz, 0, 1);
	memset(result, 0, sz + 1);
	return (void*) result;
}

// this is awesome, I love this
#ifdef MEM_TRACK
	int tallyAllocs() {
		return leftovers;
	}
	
	void* trackedAlloc(size_t typeSize, size_t amount) {
		leftovers += 1;
		// printf("allocated a new pointer, %i unmatched allocs\n", leftovers);
		return qalloc(typeSize, amount);
	}

	void trackedFree(void* obj) {
		if (obj == nullptr) return;
		leftovers -= 1;
		// printf("freed a pointer, %i unmatched allocs\n", leftovers);

		#if cachedPointers == 1
			int len = getLen(obj);
			if (freeBytes.count(len) == 0) {
				vector<void*> blocks;
				freeBytes.insert(pair<int, vector<void*>>(len, blocks));
			}
			freeBytes.at(len).push_back(obj);
		#else
			free(obj);
		#endif
	}

	void resultAllocs() {
		printf("%i unmatched allocs\n", leftovers);
	}
#else
	#if cachedPointers == 1 || useQalloc == 1
		void* trackedAlloc(size_t typeSize, size_t amount) {
			#ifdef MEM_TRACK
				leftovers += 1;
			#endif
			// printf("allocated a new pointer, %i unmatched allocs\n", leftovers);
			return qalloc(typeSize, amount);
		}

		void trackedFree(void* obj) {
			if (obj == nullptr) return;
			#ifdef MEM_TRACK
				leftovers -= 1;
			#endif

			#if cachedPointers == 1
				int len = getLen(obj);
				if (freeBytes.count(len) == 0) {
					vector<void*> blocks;
					freeBytes.insert(pair<int, vector<void*>>(len, blocks));
				}
				freeBytes.at(len).push_back(obj);
			#else
				free(obj);
			#endif
		}
	#endif

	void resultAllocs() {}
#endif

#if perfMeasure == 0
	#include <sys/time.h>
#elif perfMeasure == 1
	#include <time.h>
#elif perfMeasure == 2
	#include <chrono>
#else
	#include <profileapi.h>
#endif

long long getTimeForPerformance() {
	#if perfMeasure == 0
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		long long start = ts.tv_nsec;
		return start;
	#elif perfMeasure == 1
		clock_t clockTime = clock();
		return (((float)clockTime)/CLOCKS_PER_SEC) * 1000000000L;
	#elif perfMeasure == 2
		long long start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		return start;
	#else
		LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency); 
		QueryPerformanceCounter(&StartingTime);
		ElapsedMicroseconds.QuadPart = StartingTime.QuadPart;
		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
		return ElapsedMicroseconds.QuadPart * 100;
	#endif
}

bool startsWith(string str, string prefix) {
	for (int i = 0; i < prefix.size(); i++)
		if (str[i] != prefix[i])
			return false;
	return true;
}
