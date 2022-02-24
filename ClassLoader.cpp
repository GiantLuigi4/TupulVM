#include "ClassLoader.h"
#include "string.h"
#include "STDClasses.h"

TupulClass* getClass(ClassLoader* loader, char* name) {
	string cname = name;
	for (TupulClass* clazz : loader->classes) {
		string clazzName = clazz->name;
		if (cname == clazzName) {
			return clazz;
		}
	}
	return loadClass(loader, name);
}

TupulClass* loadClass(ClassLoader* loader, char* name) {
	// TODO:
	string nameStr = name;
	nameStr += ".txt";
	char* nameChr = (char*) nameStr.c_str();
	if (loader->sources->containsName(loader->sources, nameChr)) {
		char* chars = loader->sources->getContents(loader->sources, nameChr);
		TupulClass* clazz = passTree(loader, createClassTree((string) chars));
		loader->sources->freeSource(loader->sources, chars);
		loader->classes.push_back(clazz);
		return clazz;
	}
	TupulClass* clazz = getSTDClass(name);
	loader->classes.push_back(clazz);
	clazz->loader = loader;
	return clazz;
}

TupulClass* passTree(ClassLoader* loader, ClassTree* tree) {
	TupulClass* clazz = finishClass(tree);
	clazz->loader = loader;
	// TODO: this
	return clazz;
}

void freeLoader(ClassLoader* loader) {
	for (TupulClass* clazz : loader->classes) freeClass(clazz);
	// tree modifiers are freed by the above line
	loader->sources->freeSources(loader->sources);
}
