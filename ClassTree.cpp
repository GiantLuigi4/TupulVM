#include <string>
#include "ClassTree.h"
#include "MethodTree.h"
#include "Opcodes.h"
#include "TupulClass.h"

TupulClass* finishClass(ClassTree tree) {
    TupulClass* clazz = (TupulClass*) calloc(sizeof(TupulClass), 1);
    vector<TupulMethod*> methods;
    for (MethodTree methodTree : tree.methods) {
        methods.push_back(finishMethod(methodTree, clazz));
    }
    clazz->methods = methods;
    clazz->name = tree.name;
    return clazz;
}

ClassTree createClassTree(string str) {
    ClassTree tree;
    bool isName = false;
    string name = "";
    vector<byte> block0;
    vector<byte> block1;
    bool inBlock = false;
    bool secondBlock = false;
    byte blockCause = 0;
    vector<MethodTree> methods;

    for (char c : str) {
        byte x = (byte) c;
        // printf("%i %c\n", x, x);
        if (inBlock && should_end(blockCause, x, secondBlock)) {
            inBlock = false;
            
            //@formatter:off
            if (blockCause == METHOD) {
                MethodTree mTree = methodTreeFor(block0, block1);
                methods.push_back(mTree);
                block0.clear();
                block1.clear();
                isName = false;
                inBlock = false;
                secondBlock = false;
            }
            //@formatter:on
            
            secondBlock = false;
        }
        if (inBlock) {
            // default rust formatting is weird to me!
            //@formatter:off
            if (should_start_second_block(blockCause, x))
                { secondBlock = true; continue; }
            
            if (secondBlock) block1.push_back(x);
            else block0.push_back(x);
            //@formatter:on
            
            continue;
        }
        if (x == METHOD) {
            blockCause = x;
            isName = false;
            inBlock = true;
        }
        
        // if (x > 200) {
        //     //@formatter:off
        //     // not like I have a formatter but ok'
        //     // was originally rust code
        //          if (x == 255){printf("255 - class\n")          ;}
        //     else if (x == 254){printf("254 - method\n")         ;}
        //     else if (x == 253){printf("253 - descriptor a\n")   ;}
        //     else if (x == 246){printf("246 - end\n")            ;}
        //     else              {printf("%i c\n", x)              ;}
        //     //@formatter:on
        // }
        
        if (isName && x == DESCRIPTOR) isName = false;
        if (isName) name += (byte) x;
        if (x == CLASS) isName = true;
    }
    char* namen = (char*) calloc(sizeof(char), name.length() + 1);
    for (int i = 0; i < name.length(); i++) namen[i] = name[i];
    namen[name.length()] = 0;
    tree.name = namen;
    tree.methods = methods;
    return tree;
}
