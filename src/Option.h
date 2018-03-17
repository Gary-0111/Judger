//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_OPTION_H
#define JUDGER_OPTION_H

enum Language {
    Lang_Unknown,
    Lang_C,
    Lang_Cpp,
    Lang_Java
};

class Option {
public:
    static const unsigned long judge_time_limit   = 30000; // 30s
    static const unsigned long compile_time_limit = 30000; // 30s
    static const unsigned long spj_time_limit     = 10000; // 10s
    static const char *judge_user_name;

    void printUsage();
    int parseParameters(int argc, char* const * argv);
    void printOptions();
    Language getLanguage();
    const char* getDataDir();
    unsigned long getTimeLimit();
    unsigned long getMemoryLimit();

private:
    Language parseLanguage(const char *argstr);

private:

    Language lang;
    char data_dir[256];
    unsigned long time_limit;                   // ms
    unsigned long memory_limit;                 // KB
};

extern Option option;


#endif //JUDGER_OPTION_H
