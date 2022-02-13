#include <string>
#include <vector>
#include "MethodTree.h"
using namespace std;

#define byte unsigned char

class ClassTree {
    public: string name;
    public: vector<MethodTree> methods;
};

namespace sClassTree {
    ClassTree create(string contents);
};
