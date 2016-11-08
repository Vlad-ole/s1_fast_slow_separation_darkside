#ifndef TIME_MEASURE_H
#define TIME_MEASURE_H

#include <time.h>  //for clock_gettime

double get_time_delta(struct timespec timespec_str_before, struct timespec timespec_str_after);

#endif // TIME_MEASURE_H
