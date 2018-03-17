//
// Created by acm on 3/14/18.
//

#include <syscall.h>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <cstdlib>
#include "WhiteList.h"
#include "Logger.h"

#define Allow(id) SyscallLimit((id),
#define calls(t)  (t))
#define INF -1

// --------------- x86 ----------------
#ifdef __i386__

const char* WhiteList::syscallpath = "../syscall/syscall_x86.txt";

SyscallLimit WhiteList::cpp_limit[] = {
        Allow (SYS_read)             calls (INF),
        Allow (SYS_write)            calls (INF),
        Allow (SYS_execve)           calls (1),
        Allow (SYS_time)             calls (INF),
        Allow (SYS_access)           calls (INF),
        Allow (SYS_brk)              calls (INF),
        Allow (SYS_readlink)         calls (1),
        Allow (SYS_sysinfo)          calls (INF),
        Allow (SYS_uname)            calls (INF),
        Allow (SYS_fstat64)          calls (INF),
        Allow (SYS_set_thread_area)  calls (INF),
        Allow (SYS_exit_group)       calls (1),
        Allow (SYS_fcntl64)          calls (INF)
};

// ------------- x86_64 ---------------
#else
const char* WhiteList::syscallpath = "../syscall/syscall_x86_64.txt";

SyscallLimit WhiteList::cpp_limit[] = {
        Allow (SYS_read)             calls (INF),
        Allow (SYS_write)            calls (INF),
        Allow (SYS_fstat)            calls (INF),
        Allow (SYS_brk)              calls (INF),
        Allow (SYS_access)           calls (INF),
        Allow (SYS_execve)           calls (1),
        Allow (SYS_uname)            calls (INF),
        Allow (SYS_readlink)         calls (1),
        Allow (SYS_arch_prctl)       calls (1),
        Allow (SYS_exit_group)       calls (1),

        Allow (SYS_time)             calls (INF),
        Allow (SYS_sysinfo)          calls (INF),
        Allow (SYS_set_thread_area)  calls (INF)
};
#endif


WhiteList::WhiteList() {
    char err[1024];
    FILE *fd = fopen(syscallpath, "r");
    if(!fd) {
        sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), syscallpath);
        logger.logSysErr(err);
        exit(1);
    }
    max_syscall_id = 0;
    char str[64];
    int id;
    while(~fscanf(fd, "%s%d", str, &id)) {
        strcpy(syscall_list[id], str);
        max_syscall_id = max_syscall_id < id? id : max_syscall_id;
    }
}

void WhiteList::init() {
    memset(syscall_cnt, 0, sizeof(syscall_cnt));
    memset(white_list, 0, sizeof(white_list));
    SyscallLimit *t = cpp_limit;
    int size = sizeof(cpp_limit)/sizeof(SyscallLimit);
    for(int i = 0; i < size; i++) {
        white_list[t[i].syscall_id] = t[i].times;
    }
}

bool WhiteList::checkSyscall(int syscall_id) {
    static bool insyscall = true;

    insyscall = !insyscall;
    if(insyscall) {
        // std::clog << "Enter system call: " << syscall_list[syscall_id] << "\n";
        if(white_list[syscall_id] == INF) {
            // std::clog << syscall_list[syscall_id] << " can be called INF times.\n";
            return true;
        } else if(white_list[syscall_id] > 0) {
            white_list[syscall_id]--;
            // std::clog << syscall_list[syscall_id] << " can be called "<< white_list[syscall_id] <<" times.\n";
            return true;
        } else {
            // std::clog << syscall_list[syscall_id] << " can't be called any more!\n";
            return false;
        }
    } else {
        // std::clog << "Leave system call: " << syscall_list[syscall_id] << "\n";
        return true;
    }
}

const char *WhiteList::getSyscallName(int syscall_id) {
    return syscall_list[syscall_id];
}

void WhiteList::callSyscall(int syscall_id) {
    syscall_cnt[syscall_id]++;
}

void WhiteList::outputSyscall() {
    printf("------------- SYSCALL ---------------\n");
    for(int i = 0; i < max_syscall_id; i++) {
        if(syscall_cnt[i]) {
            printf("%s(id: %d) called %d times.\n", syscall_list[i], i, syscall_cnt[i]);
        }
    }
    printf("----------- SYSCALL END -------------\n");
}
