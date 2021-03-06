//
// Created by acm on 3/14/18.
//

#include "Clock.h"
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <cstdlib>

char Clock::time_buffer[128];

void Clock::setAlarm(int which, int milliseconds, void (*Handler)(int)) {

    switch(which) {
        case ITIMER_REAL:
            if(signal(SIGALRM, Handler) == SIG_ERR) {
                exit(1);
            }
            break;
        case ITIMER_VIRTUAL:
            if(signal(SIGVTALRM, Handler) == SIG_ERR) {
                exit(1);
            }
            break;
        case ITIMER_PROF:
            if(signal(SIGPROF, Handler) == SIG_ERR) {
                exit(1);
            }
            break;
        default:
            exit(1);
    }

    struct itimerval it;

    it.it_value.tv_sec = milliseconds / 1000;
    it.it_value.tv_usec = (milliseconds % 1000) * 1000;
    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = 0;

    if(setitimer(which, &it, NULL)) {
        exit(1);
    }
}

const char *Clock::getTime() {
    time_t raw_time;
    struct tm *info;
    //char tmp[128];

    time(&raw_time);
    info = localtime(&raw_time);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %X", info);

    return time_buffer;
}

Time Time::operator+(const Time &a) {
    Time ret;
    ret.milliseconds = this->milliseconds + a.milliseconds;
    return ret;
}

Time Time::operator-(const Time &a) {
    Time ret;
    ret.milliseconds = this->milliseconds - a.milliseconds;
    return ret;
}

bool Time::operator<(const Time &a) {
    return this->milliseconds < a.milliseconds;
}

bool Time::operator>(const Time &a) {
    return this->milliseconds > a.milliseconds;
}

Time::Time(unsigned long ms) {
    this->milliseconds = ms;
}

Time::Time(const struct timeval &a) {
    this->milliseconds = (unsigned)(a.tv_sec * 1000 + a.tv_usec / 1000);
}

