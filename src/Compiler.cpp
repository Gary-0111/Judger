//
// Created by acm on 3/14/18.
//

#include "Compiler.h"
#include "Logger.h"
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <cstring>

int Compiler::compile(const char *sourceCode) {
    int ret = 0;
    pid_t pid;
    while((pid = fork()) == -1) ;
    if(pid == 0) {
        freopen("../CE.txt", "w", stderr);

        const char * const cmd[20] = {"g++", sourceCode, "-o", "Main", "--static"};
        execvp(cmd[0], (char* const*)cmd);

        char err[1024];
        sprintf(err, "%s:%d: error: %s", __FILE__, __LINE__, strerror(errno));
        logger.logSysErr(err);

        exit(1);

    } else {
        int status;
        wait(&status);

        if(WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            logger.logNotice("Successful compile!");
        } else {
            // result = Result_CompilationError;
            printf("Exit code: %d\n", WEXITSTATUS(status));
            logger.logNotice("Compile user's code failed.");
            ret = 1;
        }
    }
    return ret;
}
