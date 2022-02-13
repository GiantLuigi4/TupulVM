#include <string>
using namespace std;

// https://stackoverflow.com/questions/8666378/detect-windows-or-linux-in-c-c/33088568
// cross platform support
#if defined(_WIN32) || defined(WIN32)
    // support for windows
    #include <stdlib> // can I move this out of the ifdef? ig mac will decide that, now that I think about it
    #define MAX_PATH _MAX_PATH // windows, why is there an _ before the variable now

    #define SEPERATOR_CHARACTER '\\'

    // fullpath method
    void fullpath(char* dst, const char* src) {
        _fullpath(full, path.c_str(), _MAX_PATH);
    }
#elif __unix__
    // support for linux
    // #include <unistd.h> // why do I need this..? do I need this..?
    #include <stdlib.h> // I think I need this here, but idk!
    #include <limits.h> // PATH_MAX
    // #define _MAX_PATH pathconf("/", 4)
    #define MAX_PATH PATH_MAX // unix, why is it PATH_MAX instead of MAX_PATH

    #define SEPERATOR_CHARACTER '/'

    // fullpath method
    // https://stackoverflow.com/questions/2341808/how-to-get-the-absolute-path-for-a-given-relative-path-programmatically-in-linux
    void fullpath(char* dst, const char* src) {
        realpath(src, dst);
    }
#else
    #define SEPERATOR_CHARACTER '/'

    // so mac doesn't have a proper way of getting the full path in C afaik, and I kinda wanted to keep this part to C
    // this works
    string fullpath(string path) {
        string abs = getWorkingDir() + "/";
        if (path.find_first_of(".") == -1) return abs + path;
        while (path.length() > 0) {
            while (path[0] == '.') {
                int indxSlash = path.find_first_of("/");
                if (indxSlash == path.find_first_not_of(".")) {
                    for (int i = 0; i < indxSlash; i++) {
                        abs = abs.substr(0, abs.find_last_of("/"));
                    }
                    path = path.substr(indxSlash + 1);
                    abs += "/";
                    // std::cout  << abs << endl << path;
                }
                indxSlash = path.find_first_of(".");
                if (indxSlash == -1) return abs + path;
            }
            int first = path.find_first_of("/");
            if  (first == -1) {
                abs += path;
                break;
            }
            abs += path.substr(0, first + 1);
            path = path.substr(first + 1);
            // std::cout << abs << endl << path;
        }
        abs = abs.replace(abs.begin(), abs.end(), "/", SEPERATOR_CHARACTER);
        return abs;
    }

    #define SEPERATOR_CHARACTER '/'
#endif

string fullpath(string path, string source) {
    if (source[source.length() - 1] != '/') source = source + "/";
    string abs = source;
    if (path.find_first_of(".") == -1) return abs + path;
    while (path.length() > 0) {
        while (path[0] == '.') {
            int indxSlash = path.find_first_of("/");
            if (indxSlash == path.find_first_not_of(".")) {
                for (int i = 0; i < indxSlash; i++) {
                    abs = abs.substr(0, abs.find_last_of("/"));
                }
                path = path.substr(indxSlash + 1);
                abs += "/";
            }
            indxSlash = path.find_first_of(".");
            if (indxSlash == -1) return abs + path;
        }
        int first = path.find_first_of("/");
        if  (first == -1) {
            abs += path;
            break;
        }
        abs += path.substr(0, first + 1);
        path = path.substr(first + 1);
    }
    abs = abs.replace(abs.begin(), abs.end(), "/", SEPERATOR_CHARACTER); // replace placeholder seperator character with intended seperator character
    return abs;
}

#include <filesystem>
string getWorkingDir() {
    return std::filesystem::current_path().generic_string();
}

using namespace std;

#define null nullptr

string absolutePath(string path) {
    #if defined(_WIN32) || defined(WIN32) || defined(__unix__)
        char* full = new char[MAX_PATH];
        fullpath(full, path.c_str());
        if (full != null) {
            string str = "";
            for (int i = 0; i < MAX_PATH; i++) if (full[i] != 0) str += full[i]; else break;
            free(full);
            return str;
        }
        __throw_logic_error("File path is invalid");
    #else
        return fullpath(path); // thank you mac... and also whatever else exists that I don't know about
    #endif
}

// 0: auto
// 1: always
// 2: vm arg
#define forgiving_fsep 0
string steralizePath(string input) {
    // vm arg: -forgiving:file:seperator
    // always: always use forgiving file seperators
    // auto: use forgiving file seperators only when not on linux
    
    // default: auto
    #if forgiving_fsep == 0
        // auto
        #if defined(_WIN32) || defined(WIN32)
            // windows supports both "\\" and "/" as file seperators as a way of being nice to the user
            // because of this fact, this has to be done

            // https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
            return input.replace(input.begin(), input.end(), "\\", "/"); // thanks windows
        #else
            return input;
        #endif
    #elif forgiving_fsep == 1
        // always
        return input.replace(input.begin(), input.end(), "\\", "/"); // thanks windows
        // TODO: vm arg mode
    #endif
}
