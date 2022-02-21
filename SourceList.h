#pragma once
#include <unordered_map>
#include <string>

using namespace std;
#define byte unsigned char

// TODO: convert to struct
class SourceList {
	public: unordered_map<char*, char*> sources;
	public: char* getContents(char*);
	public: bool containsName(char*);
};
