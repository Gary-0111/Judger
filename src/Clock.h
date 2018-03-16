//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_CLOCK_H
#define JUDGER_CLOCK_H

#include <sys/time.h>

struct Time {
    Time(unsigned long ms = 0);
    Time(const timeval &a);
    unsigned long milliseconds;
    Time operator + (const Time &a);
    Time operator - (const Time &a);
    bool operator < (const Time &a);
    bool operator > (const Time &a);
};

class Clock {
public:
    void setAlarm(int which, int milliseconds, void (*timeoutHandler)(int));
    static const char* getTime();
private:
    static char time_buffer[128];
};


#endif //JUDGER_CLOCK_H
