#include "STDClasses.h"
#include "Locals.h"

byte** print(TupulMethod* method, Locals* locals) {
    for (int i = 0; i < locals->localTypes.size(); i++) {
        byte* type = locals->localTypes[i];
        byte* local = locals->locals[i];
        switch (type[0]) {
            case 0: {
                printf("%i", local[0]);
                break;
            }
            case 1: {
                printf("%s", "NYI");
                break;
            }
            case 2: {
                printf("%s", "NYI");
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
                printf("%s", "NYI");
                break;
            }
            default: {
                printf("%s", "NYI");
                break;
            }
        }
    }
    byte** out = (byte**) calloc(sizeof(byte*), 1);
    out[0] = (byte*) calloc(sizeof(byte), 1);
    out[0][0] = (byte) 255;
    freeLocals(locals);
    return out;
}

byte** println(TupulMethod* method, Locals* locals) {
    byte** out = print(method, locals);
    printf("\n");
    return out;
}

void freeSTDMethod(TupulMethod* method) {
    free(method);
}

// 0 == sys/time (linux only)
// 1 == ctime (may not be as precise)
// 2 == chrono (may not be accurate)
// 3 == QPC (may not work in vms, windows only currently... I think there's a way to do it in linux but idk)
#ifdef __unix__
    #define defaultTimeMeasure 0
#elif defined(WIN32) || defined(_WIN32)
    #define defaultTimeMeasure 1
#else
    #define defaultTimeMeasure 1
#endif
#define timeMeasure defaultTimeMeasure

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

TupulMethod* makeSTDMethod(TupulClass* clazz, string name, string descr, byte** (*exec)(TupulMethod*,Locals*)) {
    // this just be how native methods be, yk
    TupulMethod* method = (TupulMethod*) calloc(sizeof(TupulMethod), 1);
    method->name = name;
    method->descr = descr;
    method->run = exec;
    method->free = freeSTDMethod;
    return method;
}

TupulClass* getSTDClass(char* name) {
    string namestr = name;
    if (namestr == "tupul.lang.System") {
        TupulClass* clazz = (TupulClass*) calloc(sizeof(TupulClass), 1);
        clazz->name = "tupul.lang.System";
        
        TupulMethod* method = makeSTDMethod(clazz, "print", "(*)V", print);
        clazz->methods.push_back(method);
        method = makeSTDMethod(clazz, "println", "(*)V", println);
        clazz->methods.push_back(method);

        return clazz;
    }
    // TODO: learn freaking exceptions :wheeze:
    return {0};
}
