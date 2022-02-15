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
#if defined(_WIN32) || defined(WIN32)
    #include <stringapiset.h>
#else
    #include <locale>
    #include <codecvt>
#endif
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
            char* out = (char*) calloc(sizeof(char), sizeNeeded);
            for (int i = 0; i < sizeNeeded; i++) out[i] = wstrTo[i];
        #else
            wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> converter;
            u16string utf16 = converter.from_bytes(contents);
            char* out = (char*) calloc(sizeof(char), utf16.length());
            for (int i = 0; i < utf16.length(); i++) out[i] = utf16[i];
        #endif
        
        free(contents);
        return out;
    }

    return contents;
}
