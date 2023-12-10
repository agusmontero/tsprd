#include "stopwatch.hpp"
#include <stddef.h>

StopWatch::StopWatch() {}

void StopWatch::start()
{
    gettimeofday(&_start, NULL);
}

double StopWatch::check()
{
    gettimeofday(&_end_time, NULL);
    return _end_time.tv_sec - _start.tv_sec + (_end_time.tv_usec - _start.tv_usec) / 1e6;
}