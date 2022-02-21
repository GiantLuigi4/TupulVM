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
    // if (loader->sources.containsName(name)) {
    //     TupulClass* clazz = passTree(loader, createClassTree((string) loader->sources.getContents(name)));
    //     loader->classes.push_back(clazz);
    //     return clazz;
    // }
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