#ifndef BARRUST_TIMING_H__
#define BARRUST_TIMING_H__

/*******************************************************************************
***
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***
***  Version: 1.0.4
***  Purpose: Provide a simple format to gather timing information
***
***  License: MIT 2015
***
***  Original URL: https://github.com/barrust/timing-c
***  New URL: https://github.com/barrust/c-utils
***
***  Usage:
***     Timing t;
***     timing_start(&t);
***     // code to time here
***     timing_end(&t);
***     printf("code completed in %f seconds\n", t.timing_double);
***     char *pretty_output = format_time_diff(&t);
***     printf("code completed in %s (HH:MM:SS.MS.MLS)\n", pretty_output);
***     free(pretty_output);
***
*******************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
    #include <Windows.h>
    #include <stdint.h>    /* portable: uint64_t   MSVC: __int64 */
#else
    #include <sys/time.h>  /* *nix based timing; doesn't support windows */
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TIMING_VERSION "1.0.4"
#define TIMING_MAJOR 1
#define TIMING_MINOR 0
#define TIMING_REVISION 4

#define timing_get_version()        (TIMING_VERSION)
#define timing_get_difference(t)    (t.timing_double)
#define timing_get_hours(t)         (t.hours)
#define timing_get_minutes(t)       (t.minutes)
#define timing_get_seconds(t)       (t.seconds)
#define timing_get_milliseconds(t)  (t.milliseconds)
#define timing_get_microseconds(t)  (t.microseconds)


typedef struct timing {
    struct timeval end_time;
    struct timeval start_time;
    double timing_double;
    int hours;
    short minutes;
    short seconds;
    short milliseconds;
    short microseconds;
} Timing;

/* Begin timing section */
void timing_start(Timing *t);

/* End timing section, automatically calculates time spent */
void timing_end(Timing *t);

/* Returns a time formatted as a string which must be freed by the caller */
char* format_time_diff(Timing *t);

/* Force a time difference calculation */
void calc_difference(Timing *t); /* only necessary, very occasionally */

#ifdef _WIN32
struct timeval {
    long tv_sec;
    long tv_usec;
};
int gettimeofday(struct timeval * tp, struct timezone * tzp);
#endif


/* PRIVATE FUNCTIONS */
static long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time);

/*******************************************************************************
*** PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/
void timing_start(Timing *t) {
    gettimeofday(&t->start_time, NULL);
    t->end_time.tv_sec = 0;
    t->end_time.tv_usec = 0;
    t->timing_double = 0.0;
}

void timing_end(Timing *t) {
    gettimeofday(&t->end_time, NULL);
    calc_difference(t);
}

void calc_difference(Timing *t){
    struct timeval difference;
    timeval_diff(&difference, &t->end_time, &t->start_time);
    t->timing_double = difference.tv_sec + (difference.tv_usec / 1000000.0);

    int s = difference.tv_sec;
    int ms = difference.tv_usec;
    t->hours = s / 3600;
    t->minutes = (s % 3600) / 60;
    t->seconds = (s % 3600) % 60;
    t->milliseconds = ms / 1000;
    t->microseconds = ms % 1000;
}

/*
    format_time_diff allocates memory and returns a pointer to that memory location
    NOTE: It is up to the caller to release this memory!

    E.g.,
    char *result = format_time_diff(&t);
    free(result);
*/
char* format_time_diff(Timing *t) {
    struct timeval difference;
    timeval_diff(&difference, &t->end_time, &t->start_time);
    int len = snprintf(NULL, 0, "%02d:%02d:%02d:%03d.%03d", t->hours, t->minutes, t->seconds, t->milliseconds, t->microseconds);
    char* res = (char*)calloc(len + 1, sizeof(char));
    snprintf(res, len + 1, "%02d:%02d:%02d:%03d.%03d", t->hours, t->minutes, t->seconds, t->milliseconds, t->microseconds);
    return res;
}


/*******************************************************************************
*** PRIVATE FUNCTIONS
*******************************************************************************/
static long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time) {
    struct timeval temp_diff;
    /*  if not passed in, we will use this to do the calculation and
        then return the number of microseconds */
    if(difference == NULL) {
        difference = &temp_diff;
    }
    difference->tv_sec = end_time->tv_sec - start_time->tv_sec;
    difference->tv_usec = end_time->tv_usec - start_time->tv_usec;

    /* Using while instead of if below makes the code slightly more robust. */
    while(difference->tv_usec < 0) {
        difference->tv_usec += 1000000;
        difference->tv_sec -= 1;
    }
    return 1000000LL * difference->tv_sec + difference->tv_usec;
}



/*******************************************************************************
***	Define a gettimeofday function for windows machines
*******************************************************************************/
#ifdef _WIN32
/*
    NOTE: this ignores the timezone information since we don't need it
*/
int gettimeofday(struct timeval *tp, struct timezone *tzp) {
    /* Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's */
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME	file_time;
    uint64_t	time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time =  ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}
#endif /* end defining windows gettimeofday function */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* END OF BARRUST_TIMING_H__ */
