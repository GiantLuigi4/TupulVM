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
#include <locale>
#include <codecvt>
char* readFile(string name) {
    FILE* fp;
    // println(name);
    fp = fopen(name.c_str(), "r");
    if (fp == null) {
        printf("File not found\n");
        // TODO: throw vm error
        return null;
    }
    fseek(fp, 0, SEEK_END);
    // https://stackoverflow.com/a/238607
    int len = ftell(fp);
    rewind(fp);
    char* contents = (char*) calloc(sizeof(char), len);
    fread(contents, sizeof(char), len, fp);

    // https://en.cppreference.com/w/cpp/locale/wstring_convert/from_bytes
    // TODO: get this to not throw on windows
    u16string utf16 = wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(((string) contents).data());
    char* out = (char*) calloc(sizeof(char), utf16.length());
    for (int i = 0; i < utf16.length(); i++) out[i] = utf16[i];

    fclose(fp); // fclose frees fp
    free(contents);

    return out;
}
