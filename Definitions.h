/* constants */
#define TupulByte unsigned char

/* debug options */
#define MEM_TRACK

/* defaults */

// 0 == sys/time (Linux only, Linux default)
// 1 == ctime (may not be as accurate, universal)
// 2 == chrono (not very accurate at all, universal)
#ifdef __unix__
	#define DEFAULT_TIME_MEASURE 0
#elif defined(WIN32) || defined(_WIN32)
	#define DEFAULT_TIME_MEASURE 2
#else
	#define DEFAULT_TIME_MEASURE 0
#endif

// https://stackoverflow.com/questions/12937963/get-local-time-in-nanoseconds
// http://www.cplusplus.com/reference/ctime/clock/
// https://stackoverflow.com/questions/275004/timer-function-to-provide-time-in-nano-seconds-using-c
// https://stackoverflow.com/a/12938135
// 0 == sys/time (Linux only, Linux default)
// 1 == ctime (may not be as accurate, universal)
// 2 == chrono (not very accurate at all, universal)
// 3 == QPC (Windows only, may not work in vms, should be very accurate in Windows hosts. Windows default)
#ifdef __unix__
	#define DEFAULT_PERF_MEASURE 0
#elif defined(WIN32) || defined(_WIN32)
	#define DEFAULT_PERF_MEASURE 3
#else
	#define DEFAULT_PERF_MEASURE 1
#endif

/* install options */
#define timeMeasure DEFAULT_TIME_MEASURE
#define perfMeasure DEFAULT_PERF_MEASURE

/* undefine defaults */
#undef DEFAULT_TIME_MEASURE
#undef DEFAULT_PERF_MEASURE
