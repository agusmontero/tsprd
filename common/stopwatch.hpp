#include <sys/time.h>

#ifndef STOPWATCH_H
#define STOPWATCH_H

class StopWatch
{
private:
  timeval _start, _end_time;

public:
  StopWatch();
  void start();
  double check();
};

#endif