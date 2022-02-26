#include "SourceList.h"

#include <cstdio>

// char* getContentsMap(char* name) {
// 	return this->sources[name];
// }

// bool containsNameMap(char* name) {
// 	return this->sources.count(name) != 0;
// }

// void freeSourcesMap() {
// 	for (auto src : sources) {
// 		free(src.first);
// 		free(src.second);
// 	}
// 	sources.clear();
// }
#include <string.h>

#include "Utils.h"
void freeSourcesSF(SourceList* list) {
	char** selfName = (char**) list->data;
	trackedFree(selfName[0]);
	trackedFree(selfName[1]);
	trackedFree(selfName);
	trackedFree(list);
}

void freeSourceSF(SourceList* list, char* src) {
	trackedFree(src);
}

bool containsNameSF(SourceList* list, char* name) {
	char** selfName = (char**) list->data;
	return !strcmp(name, selfName[0]);
}

char* getContentsSF(SourceList* list, char* name) {
	char** selfName = (char**) list->data;
	if (!strcmp(name, selfName[0])) return readFile(selfName[1]);
	return nullptr;
}

#include "FileSystems.h"
SourceList* createSourceSingleFile(string fileName) {
	SourceList* list = (SourceList*) trackedAlloc(sizeof(SourceList), 1);
	fileName = steralizePath(fileName);
	char** data = (char**) trackedAlloc(sizeof(char*), 2);
	
	char* path = (char*) trackedAlloc(sizeof(char), fileName.size());
	for (int i = 0; i < fileName.length(); i++) path[i] = fileName[i];

	fileName = fileName.substr(fileName.find_last_of("/") + 1);
	char* name = (char*) trackedAlloc(sizeof(char), fileName.size());
	for (int i = 0; i < fileName.length(); i++) name[i] = fileName[i];
	
	data[0] = name;
	data[1] = path;

	list->data = (long) data;
	// char** selfName = (char**) list->data;
	// printf("%s\n", selfName[0]);;
	list->freeSources = freeSourcesSF;
	list->getContents = getContentsSF;
	list->containsName = containsNameSF;
	list->freeSource = freeSourceSF;

	return list;
}

char* getContentsFS(SourceList* list, char* name) {
	char** selfName = (char**) list->data;
	string str = selfName[1];
	str += name;
	char* out = readFile(str);
	return out;
}

#include <filesystem>
bool containsNameFS(SourceList* list, char* name) {
	char** selfName = (char**) list->data;
	string str = selfName[1];
	str += name;
	return filesystem::exists(str);
}

SourceList* createSourceFS(string relDir) {
	SourceList* list = (SourceList*) trackedAlloc(sizeof(SourceList), 1);
	relDir = steralizePath(relDir);
	if (relDir[relDir.length() - 1] != '/') relDir += "/";
	char** data = (char**) trackedAlloc(sizeof(char*), 2);
	
	char* path = (char*) trackedAlloc(sizeof(char), relDir.size());
	for (int i = 0; i < relDir.length(); i++) path[i] = relDir[i];

	relDir = relDir.substr(relDir.find_last_of("/") + 1);
	char* name = (char*) trackedAlloc(sizeof(char), relDir.size());
	for (int i = 0; i < relDir.length(); i++) name[i] = relDir[i];
	
	data[0] = name;
	data[1] = path;

	list->data = (long) data;
	list->freeSources = freeSourcesSF;
	list->getContents = getContentsFS;
	list->containsName = containsNameFS;
	list->freeSource = freeSourceSF;

	return list;
}
