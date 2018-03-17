//
// Created by acm on 3/15/18.
//

#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "utility.h"
#include "Logger.h"

int cleardir(const char *dirpath) {
    DIR *dir;
    dirent *ptr;
    char err[1024];
    char tmp[1024];

    if((dir = opendir(dirpath)) == NULL) {
        sprintf(err, "%s:%d: error: %s(directory: %s)", __FILE__, __LINE__, strerror(errno), dirpath);
        logger.logSysErr(err);
        return 1;
    }
    while((ptr = readdir(dir)) != NULL) {
        if(strcmp(".", ptr->d_name) == 0 || strcmp("..", ptr->d_name) == 0) continue;

        strcpy(tmp, dirpath);
        strcat(tmp, "/");
        strcat(tmp, ptr->d_name);

        if(remove(tmp)) {
            sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), tmp);
            logger.logSysErr(err);
            return 1;
        }
    }

    closedir(dir);

    return 0;
}

bool isInputFile(const char *filename) {
    return strcmp(filename + strlen(filename) - 3, ".in") == 0;
}