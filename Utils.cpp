#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <string>

// https://stackoverflow.com/questions/8666378/detect-windows-or-linux-in-c-c/33088568
// cross platform support
#if defined(_WIN32) || defined(WIN32)
    // support for windows
    #include <stdlib> // can I move this out of the ifdef? ig mac will decide that, now that I think about it
    #define MAX_PATH _MAX_PATH // windows, why is there an _ before the variable now

    // fullpath method
    void fullpath(char* dst, const char* src) {
        _fullpath(full, path.c_str(), _MAX_PATH);
    }
#elif __unix__
    // support for linux
    #include <unistd.h> // why do I need this..? do I need this..?
    #include <stdlib.h> // I think I need this here, but idk!
    #include <limits.h> // PATH_MAX
    // #define _MAX_PATH pathconf("/", 4)
    #define MAX_PATH PATH_MAX // unix, why is it PATH_MAX instead of MAX_PATH

    // fullpath method
    // https://stackoverflow.com/questions/2341808/how-to-get-the-absolute-path-for-a-given-relative-path-programmatically-in-linux
    void fullpath(char* dst, const char* src) {
        realpath(src, dst);
    }
#endif

using namespace std;

#define null nullptr

string absolute(string path) {
    char* full = new char[MAX_PATH];
    fullpath(full, path.c_str());
    if (full != null) {
        string str = "";
        for (int i = 0; i < MAX_PATH; i++) if (full[i] != 0) str += full[i]; else break;
        return str;
    }
    return "?!`'|intendedcrashinvalidpath!?whydidInotuseapointer, lol";
}

// prints a c++ string
void print(string str) {
    printf("%s", str.c_str());
}

// prints a c++ string... but with a new line at the end!
void println(string str) {
    printf("%s\n", str.c_str());
}

// reads a file
char* readFile(string name) {
    FILE* fp;
    println(name);
    fp = fopen(name.c_str(), "r");
    if (fp == null) {
        printf("File not found\n");
        // TODO: throw vm error
        return null;
    }
    int len = fseek(fp, 0, 0);
    char* contents = (char*) calloc(sizeof(char), len);
    fread(contents, sizeof(char), len, fp);
    fclose(fp);
    return contents;
}
