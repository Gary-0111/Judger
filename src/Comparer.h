//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_COMPARER_H
#define JUDGER_COMPARER_H


#include <cstdio>
#include "Result.h"

class Comparer {
public:
    SolutionResult compare(const char *std_file, const char *usr_file);

private:
    int compareUntilNonspace(FILE *&fd_std, int &ch_std, FILE *&fd_usr, int &ch_usr);
};

extern Comparer comparer;

#endif //JUDGER_COMPARER_H
