#define _CRT_SECURE_NO_WARNINGS

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
