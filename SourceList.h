#pragma once
#include <unordered_map>
#include <string>

using namespace std;
#define byte unsigned char

// TODO: convert to struct
struct SourceList {
	// public: unordered_map<char*, char*> sources;
	public: long long data;
	public: char* (*getContents)(SourceList*,char*);
	public: bool (*containsName)(SourceList*,char*);
	public: void (*freeSource)(SourceList*,char*);
	public: void (*freeSources)(SourceList*);
};

SourceList* createSourceSingleFile(string fileName);
SourceList* createSourceFS(string relDir);
