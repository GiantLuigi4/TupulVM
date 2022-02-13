#pragma once
#include <unordered_map>
#include <string>

using namespace std;
#define byte unsigned char

class SourceList {
	public: unordered_map<string, byte*> sources;
	public: byte* getContents(string);
};
