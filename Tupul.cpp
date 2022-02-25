#include "Tupul.h"
#include "Utils.h"
#include "FileSystems.h"

#include "ClassTree.h"

using namespace std;

// [display type] == "x11" or "wayland"
// yay -Syyu glfw-[display type]

#include "Locals.h"
#include "ClassLoader.h"

#include "Utils.h"

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
		}
	}

	ClassLoader* ldr = (ClassLoader*) calloc(sizeof(ClassLoader), 1);
	// TODO: archive source
	if (launchMode == 1) ldr->sources = createSourceSingleFile(classPath);
	if (launchMode == 2) ldr->sources = createSourceFS(classPath);
	TupulClass* clazz = getClass(ldr, (char*) clazzToInvoke.c_str());
	long long start = getTimeForPerformance();
	TupulMethod* mainMethod = getMethod(clazz, (char*) methodToInvoke.c_str(), (char*) invoctionDescr.c_str());
	for (int i = 0; i < 10; i++) {
		Locals* locals = (Locals*) trackedCalloc(sizeof(Locals), 1);
		TupulByte** bytes = mainMethod->run(clazz->methods[0], locals);
		trackedFree(bytes[1]);
		trackedFree(bytes);
		resultCallocs();
	}
	Locals* locals = (Locals*) trackedCalloc(sizeof(Locals), 1);
	TupulByte** bytes = mainMethod->run(clazz->methods[0], locals);
	// should always print 2 if MEM_TRACK is enabled
	resultCallocs();

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

	long long end = getTimeForPerformance();
	long long time = end - start;
	printf("%llu nanoseconds\n", time);
	freeLoader(ldr);
	// https://stackoverflow.com/a/7619315
	return ((bytes[1][0] & 0xFF) << 24) |
		   ((bytes[1][1] & 0xFF) << 16) |
		   ((bytes[1][2] & 0xFF) << 8 ) |
		   ((bytes[1][3] & 0xFF) << 0 ) ;
}
