#include <iostream>
#include <sys/stat.h>
#include <zconf.h>
#include <cstring>
#include "Logger.h"
#include "Option.h"
#include "Compiler.h"
#include "utility.h"
#include "Executor.h"
#include "Result.h"

const char *working_dir = "temp";

int main(int argc, char *argv[]) {

    char err[1024];

    //
/*
    if(geteuid() != 0) {
        logger.logError("Please run as a root!");
    }
*/

    // Option Test

    if(option.parseParameters(argc, argv) == -1) {
        option.printUsage();
        logger.logNotice("Parse parameters failed.");
        exit(1);
    }
    option.printOptions();


    // Make the temporary directory, and change the working directory to temporary directory.
    if(mkdir(working_dir, 0777)) {
        sprintf(err, "%s:%d: error: %s(directory: %s)", __FILE__, __LINE__, strerror(errno), working_dir);
        logger.logSysErr(err);
        exit(1);
    }
    if(chdir(working_dir)) {
        sprintf(err, "%s:%d: error: %s(directory: %s)", __FILE__, __LINE__, strerror(errno), working_dir);
        logger.logSysErr(err);
        exit(1);
    }

    // Compiler Test
    // g++ ../Main.cpp -o Main --static
    Compiler compiler;
    compiler.compile("../test/Main.cpp");


    Executor executor;
    char exe_path[1024];
    strcpy(exe_path, working_dir);
    strcat(exe_path, "/Main");
    executor.execute(exe_path);


    // Change the working directory to parent directory, and remove the temporary directory.
    if(chdir("..")) {
        sprintf(err, "%s:%d: error: %s(directory: %s)", __FILE__, __LINE__, strerror(errno), "..");
        logger.logSysErr(err);
        exit(1);
    }

    if(cleardir(working_dir)) {
        exit(1);
    }
    if(rmdir(working_dir)) {
        sprintf(err, "%s:%d: error: %s(directory: %s)", __FILE__, __LINE__, strerror(errno), working_dir);
        logger.logSysErr(err);
        exit(1);
    }

    // Logger Test
    /*
    logger.logNotice("Notice test!!!");
    logger.logError("WTF!!!!");
    logger.logWarning("FBI Warning!");
    */

    return 0;
}