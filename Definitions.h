#pragma once
/* constants */
#define TupulByte unsigned char
#define null nullptr

/* debug options */
// #define MEM_TRACK
#define MEM_TRACK_COUNT

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

/* start: Time Options */
/* 
	Setting:
		Label: Time Measurement Method
		Options:
			Option: DEFAULT_TIME_MEASURE
				Name: Default
			Option: 0
				Name: SysTime
				Description: Uses the SysTime API, which is a linux exclusive feature
				Requirements: Linux
			Option: 1
				Name: ctime
				Description: Portable, but not very accurate
			Option: 2
				Name: chrono
				Description: Portable, but not very accurate
*/
#define timeMeasure DEFAULT_TIME_MEASURE
/*
	Setting:
		Label: Performance Measurement Method
		Options:
			Option: DEFAULT_PERF_MEASURE
				Name: Default
			Option: 0
				Name: SysTime
				Description:
					Uses the SysTime API, which is a Unix exclusive feature
					Very accurate
					Default for Unix
				Requirements: unix
			Option: 1
				Name: ctime
				Description:
					Portable, but not very accurate
					Default for anything which is neither Unix nor Windows
			Option: 2
				Name: chrono
				Description: Portable, but not very accurate
			Option: 3
				Name: QPC
				Description:
					Query Performance Counters, rather accurate but Windows exclusive
					Default for windows
				Requirements: Windows
*/
#define perfMeasure DEFAULT_PERF_MEASURE

/* start: Memory Options */
/*
	Setting:
		Label: Cached Pointers
		Description:
			Should the VM cache pointers?
			Reduces allocation calls, which makes the program not cause fans to spin up as much
			Will likely reduce the speed of the program
			May or may not work properly, not fully tested
		Options:
			Option: 1
				Name: Default (On)
			Option: 0
				Name: Off
			Option: 1
				Name: On
*/
// TODO: in the future, make this default to off
#define cachedPointers 1
/*
	Setting:
		Label: Qalloc
		Description:
			Should the VM use a custom calloc (allocation) method?
			Tends to wind up working better
			The VM may crash frequently without this
			If Cached Pointers are enabled, this is also enabled
		Options:
			Option: 1
				Name: Default (On)
			Option: 0
				Name: Off
			Option: 1
				Name: On
*/
#define useQalloc 1
// TODO: custom memory allocator

/* undefine invalid options */
#ifndef MEM_TRACK
#undef MEM_TRACK_COUNT
#endif