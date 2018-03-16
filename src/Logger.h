//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_LOGGER_H
#define JUDGER_LOGGER_H

#include <stdio.h>

class Logger {
public:
    Logger();
    ~Logger();
    void logNotice(const char *info);
    void logError(const char *err);
    void logSysErr(const char *serr);
    void logWarning(const char *warn);
private:
    static const char *log_file;
    FILE *log_stream;
};

extern Logger logger;


#endif //JUDGER_LOGGER_H
