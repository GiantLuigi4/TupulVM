#include <string>
#include "ClassTree.h"
#include "MethodTree.h"
#include "Opcodes.h"

ClassTree sClassTree::create(string str) {
    ClassTree tree;
    bool isName = false;
    string name = "";
    vector<byte> block0;
    vector<byte> block1;
    bool inBlock = false;
    bool secondBlock = false;
    byte blockCause = 0;
    vector<MethodTree> methods; // TODO:

    for (char c : str) {
        byte x = (byte) c;
        printf("%i %c\n", x, x);
        if (inBlock && should_end(blockCause, x, secondBlock)) {
            inBlock = false;
            
            //@formatter:off
            if (blockCause == METHOD) {
                MethodTree mTree = methodTreeFor(block0, block1);
                methods.push_back(mTree);
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
        
        if (x > 200) {
            //@formatter:off
            // rust is annoying in the fact that it requires the curlies
            // this means I can't format my code quite how I'd like to
                 if (x == 255){printf("255 - class\n")          ;}
            else if (x == 254){printf("254 - method\n")         ;}
            else if (x == 253){printf("253 - descriptor\n")     ;}
            else if (x == 246){printf("246 - end\n")            ;}
            else            {printf("%i\n", x)                  ;}
            //@formatter:on
        }
        
        if (isName && x == DESCRIPTOR) isName = false;
        if (isName) name += (byte) x;
        if (x == CLASS) isName = true;
    }
    tree.name = name;
    tree.methods = methods;
    return tree;
}