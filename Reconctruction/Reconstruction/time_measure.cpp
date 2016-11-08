#include "time_measure.h"

double get_time_delta(struct timespec timespec_str_before, struct timespec timespec_str_after)
{
    return (timespec_str_after.tv_sec + timespec_str_after.tv_nsec*1e-9 ) - (timespec_str_before.tv_sec + timespec_str_before.tv_nsec*1e-9 );
}
