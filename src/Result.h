//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_RESULT_H
#define JUDGER_RESULT_H

#include "Clock.h"

enum SolutionResult {
    Result_Running,             // := 0
    Result_Accepted,            // := 1
    Result_WrongAnswer,         // := 2
    Result_TimeLimitExceed,     // := 3
    Result_MemoryLimitExceed,   // := 4
    Result_OutputLimitExceed,   // := 5
    Result_RuntimeError,        // := 6
    Result_PresentationError,   // := 7
    Result_CompilationError,    // := 8
    Result_SystemError,         // := 9
    Result_DangerouCode         // := 10
};

extern const char *result_str[];


class Result {
public:
    Result();
    void setResult(SolutionResult _res);
    SolutionResult getResult();
    void updateUsedTime(Time _usedTime);
    void setUsedTime(Time _usedTime);
    void updateUsedMemory(unsigned long _usedMem);
    void printResult();
    bool memoryExceed(unsigned long _memLim);
    bool timeExceed(unsigned long _timeLim);
    Time getUsedTime();
private:
    SolutionResult res;
    Time used_time;
    unsigned long used_memory;
};

extern Result result;


#endif //JUDGER_RESULT_H
