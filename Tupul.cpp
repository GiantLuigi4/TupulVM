#include "Tupul.h"
#include "Utils.h"
#include "FileSystems.h"

#include "loading/ClassTree.h"

using namespace std;

// [display type] == "x11" or "wayland"
// yay -Syyu glfw-[display type]

#include "Locals.h"
#include "loading/ClassLoader.h"

#include "Utils.h"

#include "data/Types.h"

int main(int argc, char** args) {
	// args[0] // executable path
	
	// launch mode:
	// 0 == archive
	// 1 == single file
	// 2 == file system
	int launchMode = 0;
	// launch args
	string classPath = "";
	string clazzToInvoke = "";
	string methodToInvoke = "";
	string invoctionDescr = "";
	bool debugOut = true;
	for (int i = 1; i < argc; i++) {
		string argStr = args[i];
		if (startsWith(argStr, "-mode:")) {
			string md = argStr.substr(argStr.find_first_of(":") + 1);
			if (md == "archive") launchMode = 0;
			else if (md == "single_file") launchMode = 1;
			else if (md == "sf") launchMode = 1;
			else if (md == "file_system") launchMode = 2;
			else if (md == "file_sys") launchMode = 2;
			else if (md == "fs") launchMode = 2;
		} else if (startsWith(argStr, "-class_path:") || startsWith(argStr, "-cp:")) {
			string cp = argStr.substr(argStr.find_first_of(":") + 1);
			classPath = cp;
		} else if (startsWith(argStr, "-class:")) {
			string clazz = argStr.substr(argStr.find_first_of(":") + 1);
			clazzToInvoke = clazz;
		} else if (startsWith(argStr, "-method")) {
			string method = argStr.substr(argStr.find_first_of(":") + 1);
			invoctionDescr = method.substr(method.find_first_of("("));
			method = method.substr(0, method.find_first_of("("));
			methodToInvoke = method;
		} else if (startsWith(argStr, "--debugOutput") || startsWith(argStr, "--debugOut") || startsWith(argStr, "--dout")) debugOut = true;
	}

	ClassLoader* ldr = (ClassLoader*) trackedAlloc(sizeof(ClassLoader), 1);
	// TODO: archive source
	if (launchMode == 1) ldr->sources = createSourceSingleFile(classPath);
	if (launchMode == 2) ldr->sources = createSourceFS(classPath);
	TupulClass* clazz = getClass(ldr, (char*) clazzToInvoke.c_str());
	TupulMethod* mainMethod = getMethod(clazz, (char*) methodToInvoke.c_str(), (char*) invoctionDescr.c_str());
	for (int i = 0; i < 100000000; i++) {
		Locals* locals = (Locals*) trackedAlloc(sizeof(Locals), 1);
		TupulByte** bytes = mainMethod->run(clazz->methods[0], locals);
		trackedFree(bytes[1]);
		trackedFree(bytes);
		// resultAllocs();
	}
	Locals* locals = (Locals*) trackedAlloc(sizeof(Locals), 1);
	unsigned long long start = getTimeForPerformance();
	TupulByte** bytes = mainMethod->run(clazz->methods[0], locals);
	unsigned long long end = getTimeForPerformance();
	unsigned long long time = end - start;
	printf("%llu nanoseconds\n", time);

	if (bytes[0][0] == 254) { // vmerr
		switch (bytes[1][0]) {
			case 1:
				switch (bytes[1][1]) {
					// math operator unimplemented
					case 0: return -1;
					// no error found
					default: return -21;
				}
			// no error table found
			default: return -20;
		}
	}
	int i0 = *((int*) bytes[1]);

	if (debugOut) {
		if (bytes[0] == LONG) printf("-- Result: %lli --\n", *((long long*) bytes[1]));
		if (bytes[0] == INT) printf("-- Result: %i --\n", *((int*) bytes[1]));
		if (bytes[0] == SHORT) printf("-- Result: %i --\n", *((int*) bytes[1]));
		if (bytes[0] == BYTE) printf("-- Result: %i --\n", (int) *((TupulByte*) bytes[1]));
		if (bytes[0] == FLOAT) printf("-- Result: %f --\n", *((float*) bytes[1]));
		if (bytes[0] == DOUBLE) printf("-- Result: %f --\n", *((double*) bytes[1]));
		if (bytes[0] == CHAR) printf("-- Result: %c --\n", *((char*) bytes[1]));
	}

	trackedFree(bytes[1]);
	trackedFree(bytes);

	freeLoader(ldr);
	// should always print 0 if MEM_TRACK is enabled
	resultAllocs();
	// https://stackoverflow.com/a/7619315
	return i0;
	// return 0;
}
