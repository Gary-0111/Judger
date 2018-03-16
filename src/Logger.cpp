//
// Created by acm on 3/14/18.
//

#include "Logger.h"
#include "Clock.h"
#include <stdio.h>
#include <stdlib.h>

Logger logger;

const char* Logger::log_file = "./log/Judger.log";

Logger::Logger() {
    if((log_stream = fopen(log_file, "a")) == NULL) {
        printf("Can not open log_file: %s\n", log_file);
        exit(1);
    }
}

void Logger::logNotice(const char *info) {
    fprintf(log_stream, "[%s] | [NOTICE]==> %s\n", Clock::getTime(), info);
}

void Logger::logError(const char *err) {
    fprintf(log_stream, "[%s] | [ERROR]==> %s\n", Clock::getTime(), err);
}

void Logger::logSysErr(const char *serr) {
    fprintf(log_stream, "[%s] | [SYSTEM ERROR]==> %s\n", Clock::getTime(), serr);
}

void Logger::logWarning(const char *warn) {
    fprintf(log_stream, "[%s] | [WARNING]==> %s\n", Clock::getTime(), warn);
}

Logger::~Logger() {
    if(log_stream) {
        fclose(log_stream);
    }
}


