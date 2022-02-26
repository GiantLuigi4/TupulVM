#include "STDClasses.h"
#include "Locals.h"

#include "Utils.h"

TupulByte** print(TupulMethod* method, Locals* locals) {
	for (int i = 0; i < locals->localTypes.size(); i++) {
		TupulByte* type = locals->localTypes[i];
		TupulByte* local = locals->locals[i];
		switch (type[0]) {
			case 0: {
				printf("%i", local[0]);
				break;
			}
			case 1: {
				// https://stackoverflow.com/q/5134779
				short i = (short) ((local[0] & 0xFF) << 8) |
								  ((local[1] & 0xFF)     ) ;
				printf("%i", i);
				break;
			}
			case 2: {
				short i = (short) ((local[0] & 0xFF) << 8) |
								  ((local[1] & 0xFF)     ) ;
				printf("%c", i);
				break;
			}
			case 3: {
				printf("%s", "NYI");
				break;
			}
			case 4: {
				int i = (int) ((local[0] & 0xFF) << 24) |
							  ((local[1] & 0xFF) << 16) |
							  ((local[2] & 0xFF) <<  8) |
							  ((local[3] & 0xFF) <<  0) ;
				printf("%i", i);
				break;
			}
			case 5: {
				printf("%s", "NYI");
				break;
			}
			case 6: {
				long long i0 = (((long long) local[0] & 0xFF) << 56) |
							   (((long long) local[1] & 0xFF) << 48) |
							   (((long long) local[2] & 0xFF) << 40) |
							   (((long long) local[3] & 0xFF) << 32) |
							   (((long long) local[4] & 0xFF) << 24) |
							   (((long long) local[5] & 0xFF) << 16) |
							   (((long long) local[6] & 0xFF) <<  8) |
							   (((long long) local[7] & 0xFF) <<  0) ;
				printf("%lld", i0);
				break;
			}
			default: {
				printf("%s", "NYI");
				break;
			}
		}
	}
	TupulByte** out = (TupulByte**) trackedAlloc(sizeof(TupulByte*), 1);
	out[0] = (TupulByte*) trackedAlloc(sizeof(TupulByte), 1);
	out[0][0] = (TupulByte) 255;
	freeLocals(locals);
	return out;
}

TupulByte** println(TupulMethod* method, Locals* locals) {
	TupulByte** out = print(method, locals);
	printf("\n");
	return out;
}

void freeSTDMethod(TupulMethod* method) {
	// nothing to do
}

#include "Definitions.h"

#if timeMeasure == 0
	#include <sys/time.h>
#elif timeMeasure == 1
	#include <time.h>
#else
	#include <chrono>
#endif

long long getTime() {
	#if timeMeasure == 0
		timespec ts;
		clock_gettime(CLOCK_REALTIME, &ts);
		long long start = ts.tv_nsec;
		return start;
	#elif timeMeasure == 1
		clock_t clockTime = clock();
		return (((float)clockTime)/CLOCKS_PER_SEC) * 1000000000L;
	#else
		long long start = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		return start;
		// TODO: QPC
	#endif
}

#include "Types.h"
TupulByte** getTimeT(TupulMethod* method, Locals* locals) {
	freeLocals(locals);
	long long time = getTime();

	TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 8);
	out[0] = (time >> (long long) 56) & 0xFF;
	out[1] = (time >> (long long) 48) & 0xFF;
	out[2] = (time >> (long long) 40) & 0xFF;
	out[3] = (time >> (long long) 32) & 0xFF;
	out[4] = (time >> 24) & 0xFF;
	out[5] = (time >> 16) & 0xFF;
	out[6] = (time >> 8) & 0xFF;
	out[7] = time & 0xFF;
	
	TupulByte** realOut = (TupulByte**) trackedAlloc(sizeof(TupulByte*), 2);
	realOut[1] = out;
	realOut[0] = LONG;
	return realOut;
}
TupulByte** getTimeM(TupulMethod* method, Locals* locals) {
	freeLocals(locals);
	long long time = getTime();

	TupulByte* out = (TupulByte*) trackedAlloc(sizeof(TupulByte), 8);
	out[0] = (time >> (long long) 56) & 0xFF;
	out[1] = (time >> (long long) 48) & 0xFF;
	out[2] = (time >> (long long) 40) & 0xFF;
	out[3] = (time >> (long long) 32) & 0xFF;
	out[4] = (time >> 24) & 0xFF;
	out[5] = (time >> 16) & 0xFF;
	out[6] = (time >> 8) & 0xFF;
	out[7] = time & 0xFF;
	
	TupulByte** realOut = (TupulByte**) trackedAlloc(sizeof(TupulByte*), 2);
	realOut[1] = out;
	realOut[0] = LONG;
	return realOut;
}

TupulMethod* makeSTDMethod(TupulClass* clazz, string name, string descr, TupulByte** (*exec)(TupulMethod*,Locals*)) {
	// this just be how native methods be, yk
	TupulMethod* method = (TupulMethod*) trackedAlloc(sizeof(TupulMethod), 1);
	method->name = name;
	method->descr = descr;
	method->run = exec;
	method->free = freeSTDMethod;
	return method;
}

TupulClass* getSTDClass(char* name) {
	string namestr = name;
	if (namestr == "tupul.lang.System") {
		TupulClass* clazz = (TupulClass*) trackedAlloc(sizeof(TupulClass), 1);
		clazz->name = (char*) "tupul.lang.System";
		
		TupulMethod* method = makeSTDMethod(clazz, "print", "(*)V", print);
		clazz->methods.push_back(method);
		method = makeSTDMethod(clazz, "println", "(*)V", println);
		clazz->methods.push_back(method);
		method = makeSTDMethod(clazz, "nanoTime", "()L", getTimeT);
		clazz->methods.push_back(method);
		method = makeSTDMethod(clazz, "milliTime", "()L", getTimeM);
		clazz->methods.push_back(method);

		return clazz;
	}
	// TODO: learn freaking exceptions :wheeze:
	return {0};
}
