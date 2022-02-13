// Tupul.cpp : Defines the entry point for the application.
//

#include "Tupul.h"
#include "Utils.h"
#include "FileSystems.h"

#include "ClassTree.h"

using namespace std;

int main(int argc, char** args) {
	// println(absolutePath(".vscode/BasicTest.txt"));
	string str = readFile(".vscode/BasicTest.txt");
	// string str = readFile("Tupul.cpp");
	ClassTree tree = sClassTree::create(str);
	println(tree.name);
	printf("%i\n", tree.methods.size());
		println(tree.methods[0].name);
		println(tree.methods[0].descr);
	return 0;
}
