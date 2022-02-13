// Tupul.cpp : Defines the entry point for the application.
//

#include "Tupul.h"
#include "Utils.h"
#include "FileSystems.h"

using namespace std;

int main(int argc, char** args) {
	println(getWorkingDir().substr(0, getWorkingDir().find_last_of("/")));
	string test = "./hello.txt";
	test = test.substr(test.find_first_of("/") + 1);
	println(test);
	println(absolutePath("test.txt"));
	println(absolutePath("../test.txt"));
	return 0;
}
