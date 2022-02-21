#include "Tupul.h"
#include "Utils.h"
#include "FileSystems.h"

#include "ClassTree.h"

using namespace std;

// [display type] == "x11" or "wayland"
// yay -Syyu glfw-[display type]

#include "Locals.h"
#include "ClassLoader.h"

int main(int argc, char** args) {
	// println(absolutePath(".vscode/BasicTest.txt"));
	string str = readFile(".vscode/BasicTest.txt");
	// string str = readFile("Tupul.cpp");
	ClassTree* tree = createClassTree(str);
	// println(tree.name);
	// printf("%i\n", tree.methods.size());
	// println(tree.methods[0].name);
	// println(tree.methods[0].descr);
	ClassLoader* ldr = (ClassLoader*) calloc(sizeof(ClassLoader), 1);
	TupulClass* clazz = finishClass(tree);
	clazz->loader = ldr;
	long long start = getTimeForPerformance();
	Locals* locals = (Locals*) calloc(sizeof(Locals), 1);
	byte** bytes = clazz->methods[0]->run(clazz->methods[0], locals);
	long long end = getTimeForPerformance();
	long long time = end - start;
	printf("%i nanoseconds\n", time);
	clazz->methods[0]->free(clazz->methods[0]);
	// https://stackoverflow.com/a/7619315
	return ((bytes[1][0] & 0xFF) << 24) |
			((bytes[1][1] & 0xFF) << 16) |
			((bytes[1][2] & 0xFF) << 8) |
			((bytes[1][3] & 0xFF) << 0);
}
