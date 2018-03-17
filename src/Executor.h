//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_EXECUTOR_H
#define JUDGER_EXECUTOR_H

#include "WhiteList.h"

class Executor {
public:
    int execute(const char *exe_path);
private:
    void IORedirect(const char *std_input_file, const char *usr_output_file);
    unsigned long getMemory(pid_t pid);
    void setLimit();

    WhiteList whitelist;
};


#endif //JUDGER_EXECUTOR_H
