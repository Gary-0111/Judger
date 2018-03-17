//
// Created by acm on 3/14/18.
//

#include <cstdio>
#include "Result.h"

#define max(a,b) a<b?b:a

Result result;

const char *result_str[] = {
        "Running",
        "Accepted",
        "Wrong Answer",
        "Time Limit Exceed",
        "Memory Limit Exceed",
        "Output Limit Exceed",
        "Runtime Error",
        "Presentation Error",
        "Compilation Error",
        "System Error",
        "Dangerous Code"
};

Result::Result() {

}

void Result::setResult(SolutionResult _res) {
    res = _res;
}

SolutionResult Result::getResult() {
    return res;
}

void Result::updateUsedTime(Time _usedTime) {
    used_time = used_time + _usedTime;
}

void Result::updateUsedMemory(unsigned long _usedMem) {
    used_memory = max(used_memory, _usedMem);
}

void Result::printResult() {
    printf("#################### Result ###################\n");
    printf("%s\n", result_str[res]);
    printf("Used time: %lums    Used memory: %luKB\n", used_time.milliseconds, used_memory);
}

bool Result::memoryExceed(unsigned long _memLim) {
    return used_memory > _memLim;
}

bool Result::timeExceed(unsigned long _timeLim) {
    return used_time > _timeLim;
}

Time Result::getUsedTime() {
    return used_time;
}

void Result::setUsedTime(Time _usedTime) {
    used_time = _usedTime;
}
