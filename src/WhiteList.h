//
// Created by acm on 3/14/18.
//

#ifndef JUDGER_WHITLIST_H
#define JUDGER_WHITLIST_H


struct SyscallLimit {
    short syscall_id, times;
    SyscallLimit(short id, short t): syscall_id(id), times(t) {}
};

class WhiteList {
public:
    WhiteList();
    void init();
    bool checkSyscall(int syscall_id);
    const char *getSyscallName(int syscall_id);
    void callSyscall(int syscall_id);
    void outputSyscall();

private:
    static const char *syscallpath;
    static SyscallLimit cpp_limit[];

    char syscall_list[400][64];
    int max_syscall_id;
    int syscall_cnt[400];
    short white_list[512];
};


#endif //JUDGER_WHITLIST_H
