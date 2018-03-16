//
// Created by acm on 3/14/18.
//

#include <cctype>
#include <errno.h>
#include <cstring>
#include "Comparer.h"
#include "Logger.h"

Comparer comparer;

SolutionResult Comparer::compare(const char *std_file, const char *usr_file) {
    char err[1024];
    SolutionResult ret = Result_Running;
    FILE *fd_std = fopen(std_file, "r");
    FILE *fd_usr = fopen(usr_file, "r");

    if(!fd_std) {
        sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), std_file);
        logger.logSysErr(err);
        return Result_SystemError;
    }
    if(!fd_usr) {
        sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), usr_file);
        logger.logSysErr(err);
        return Result_SystemError;
    }

    bool isEnd = false;
    int ch_std = fgetc(fd_std);
    int ch_usr = fgetc(fd_usr);
    while(ret == Result_Running) {
        if(isEnd) break;
        if(compareUntilNonspace(fd_std, ch_std, fd_usr, ch_usr) != 0) {
            ret = Result_PresentationError;
            break;
        }
        while((!isspace(ch_std)) && (!isspace(ch_usr))) {
            if(ch_std == EOF && ch_usr == EOF) {
                isEnd = true;
                break;
            }
            if(ch_std != ch_usr) {
                ret = Result_WrongAnswer;
                break;
            }
            ch_std = fgetc(fd_std);
            ch_usr = fgetc(fd_usr);
        }
    }

    if(fd_std) fclose(fd_std);
    if(fd_usr) fclose(fd_usr);

    return ret;
}

int Comparer::compareUntilNonspace(FILE *&fd_std, int &ch_std, FILE *&fd_usr, int &ch_usr) {
    while(isspace(ch_std) || isspace(ch_usr)) {
        if(ch_std != ch_usr) {
            // Deal with the files from Windows.
            // The end-of-line is CRLF(\r\n) in Windows, LF(\n) in *nix and CR("\r") in Mac.
            if(ch_std == '\r' && ch_usr == '\n') {
                ch_std = fgetc(fd_std);
                if(ch_std != ch_usr) {
                    return 1;
                }
            } else {
                return 1;
            }
        }
        if(isspace(ch_std)) ch_std = fgetc(fd_std);
        if(isspace(ch_usr)) ch_usr = fgetc(fd_usr);
    }
    return 0;
}
