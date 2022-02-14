#include "Tupul.h"
#include "Utils.h"
#include "FileSystems.h"

#include "ClassTree.h"

#include <chrono>

using namespace std;

int main(int argc, char** args) {
	// https://stackoverflow.com/a/12938135
	long long start = std::chrono::system_clock::now().time_since_epoch().count();

	// println(absolutePath(".vscode/BasicTest.txt"));
	string str = readFile(".vscode/BasicTest.txt");
	// string str = readFile("Tupul.cpp");
	ClassTree tree = createClassTree(str);
	// println(tree.name);
	// printf("%i\n", tree.methods.size());
	// println(tree.methods[0].name);
	// println(tree.methods[0].descr);
	TupulClass clazz = finishClass(tree);
	byte** bytes = clazz.methods[0]->run(clazz.methods[0]);
	long long end = std::chrono::system_clock::now().time_since_epoch().count();
	printf("%i\n", end - start); 
	clazz.methods[0]->free(clazz.methods[0]);
	// https://stackoverflow.com/a/7619315
	return ((bytes[1][0] & 0xFF) << 24) |
		((bytes[1][1] & 0xFF) << 16) |
		((bytes[1][2] & 0xFF) << 8) |
		((bytes[1][3] & 0xFF) << 0);
}
