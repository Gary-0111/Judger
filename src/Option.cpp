//
// Created by acm on 3/14/18.
//

#include "Option.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "Logger.h"

Option option;

const char* Option::judge_user_name = "huang";

const char *LangStr[] = {
        "Unknown",
        "C",
        "C++",
        "Java"
};

void Option::printUsage() {
    printf("Usage: ./Judge -l language -d data_directory           \n");
    printf("               -t time_limit -m memory_limit           \n");
    printf("                                                       \n");
    printf("Options:                                               \n");
    printf("     -l      The solution's language [C|C++|Java]      \n");
    printf("     -d      Data's directory.                         \n");
    printf("     -t      Time limit. Millisecond by default.       \n");
    printf("     -m      Memory limit. Kilo-Bytes by default. [K|M]\n");
}

int Option::parseParameters(int argc, char * const * argv) {
    int ch;
    while((ch = getopt(argc, argv, "l:d:t:m:")) != -1) {
        switch(ch) {
            case 'l':
                if((lang = parseLanguage(optarg)) == Lang_Unknown) return -1;
                break;
            case 'd':
                strcpy(data_dir, optarg);
                break;
            case 't':
                sscanf(optarg, "%lu", &time_limit);
                break;
            case 'm':
                sscanf(optarg, "%lu", &memory_limit);
                break;
            case '?':
                fprintf(stderr, "Invalid options -- '%c'\n", optopt);
                return -1;
            default:
                break;
        }
    }
    if(argc != 9) return -1;
    return 0;
}

Language Option::getLanguage() {
    return lang;
}

const char *Option::getDataDir() {
    return data_dir;
}

unsigned long Option::getTimeLimit() {
    return time_limit;
}

unsigned long Option::getMemoryLimit() {
    return memory_limit;
}

Language Option::parseLanguage(const char *argstr) {
    if(strcmp(argstr, "C") == 0) return Lang_C;
    else if(strcmp(argstr, "C++") == 0) return Lang_Cpp;
    else if(strcmp(argstr, "Java") == 0) return Lang_Java;
    return Lang_Unknown;
}

void Option::printOptions() {
    printf("Language:        %s\n", LangStr[lang]);
    printf("Data directory:  %s\n", data_dir);
    printf("Time Limit:      %lu ms\n", time_limit);
    printf("Memory Limit:    %lu KB\n", memory_limit);
}
