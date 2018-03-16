//
// Created by acm on 3/14/18.
//

#include <string>
#include <unistd.h>
#include <dirent.h>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <cstdlib>
#include <pwd.h>
#include "Executor.h"
#include "Option.h"
#include "Logger.h"
#include "Clock.h"
#include "Result.h"
#include "utility.h"
#include "Comparer.h"
#include <sys/ptrace.h>
#include <wait.h>
#include <sys/reg.h>
#include <sys/resource.h>

extern Option option;
extern Result result;

int Executor::execute(const char *exe_path) {

    int ret = 0;
    char err[1024];
    pid_t pid = -1;

    DIR *dir;
    dirent *ptr;
    char data_dir[1024];
    strcpy(data_dir, "../");
    strcat(data_dir, option.getDataDir());
    if((dir = opendir(data_dir)) == NULL) {
        sprintf(err, "%s:%d: error: %s(directory: %s)", __FILE__, __LINE__, strerror(errno), option.getDataDir());
        exit(1);
    }

    passwd* judge_user = getpwnam(option.judge_user_name);
    if(judge_user == NULL) {
        sprintf(err, "%s:%d: error: %s(No user named: %s)", __FILE__, __LINE__, strerror(errno), option.judge_user_name);
        logger.logSysErr(err);
        exit(1);
    }

    char tmpName[1024];

    // Traverse all input files in dirpath.
    while((result.getResult() == Result_Running || result.getResult() == Result_PresentationError)
          && (ptr = readdir(dir)) != NULL)
    {

        if(strcmp(".", ptr->d_name) == 0 || strcmp("..", ptr->d_name) == 0) continue;
        if(!isInputFile(ptr->d_name)) continue;

        int len = strlen(ptr->d_name);
        strcpy(tmpName, ptr->d_name);
        tmpName[len - 3] = '\0';

        char std_input_file[256], std_output_file[256], usr_output_file[256];
        strcpy(std_input_file, (std::string(data_dir) + "/" + ptr->d_name).c_str());
        strcpy(std_output_file, (std::string(data_dir) + "/" + tmpName + ".out").c_str());
        strcpy(usr_output_file, (std::string(tmpName) + ".out").c_str());

        fprintf(stderr, "\n************** Test case # %s *****************\n", tmpName);
        fprintf(stderr, "standard input file:  %s\n", std_input_file);
        fprintf(stderr, "standard output file: %s\n", std_output_file);
        fprintf(stderr, "user's output file:   %s\n", usr_output_file);

        while((pid = fork()) == -1) ;

        if(pid == 0) {

            // I/O redirect.
            IORedirect(std_input_file, usr_output_file);

            // Set euid.
            if(seteuid(judge_user->pw_uid) != EXIT_SUCCESS) {
                sprintf(err, "%s:%d: error: %s(judge_user: %s)", __FILE__, __LINE__, strerror(errno), option.judge_user_name);
                logger.logSysErr(err);
                exit(1);
            }

            fprintf(stderr, "The Judger's id is %d.\n", judge_user->pw_uid);
            fprintf(stderr, "The child process's uid is %d.\n", getuid());
            fprintf(stderr, "The child process's euid is %d.\n", geteuid());

            // Trace the child process.
            ptrace(PTRACE_TRACEME, 0, NULL, NULL);

            // Set the resources limits.
            setLimit();

            // signal(SIGPROF, timeoutHandler);
            // alarm(ITIMER_REAL, 1000);

            // Execute the user's program.
            execvp("./Main", NULL);

            sprintf(err, "%s:%d: error: %s", __FILE__, __LINE__, strerror(errno));
            logger.logSysErr(err);

            exit(1);
        } else {
            // 监控子进程的系统调用,并监测子进程使用的内存及时间
            int status;
            /*
            memset(syscall_cnt, 0, sizeof(syscall_cnt));
            initWhiteList();
            outputWhiteList();
             */
            Time caseTime;
            struct rusage rused;
            while (true) {
                wait4(pid, &status, 0, &rused);

                // Get the system call ID.
                int syscall_id = ptrace(PTRACE_PEEKUSER, pid, 4 * ORIG_EAX, NULL);
                //syscall_cnt[syscall_id]++;

                // Check if the child process is terminated normally.
                if(WIFEXITED(status)) {
                    //outputSyscall();
                    fprintf(stderr, "User's process was terminated normally.\n");

                    SolutionResult tmpRes = comparer.compare(std_output_file, usr_output_file);
                    result.setResult(tmpRes);

                    break;
                }

                // Check if the child process is TLE, RE or OLE.
                if(WIFSIGNALED(status) ||
                   (WIFSTOPPED(status) && WSTOPSIG(status) != SIGTRAP))
                {
                    int signo = 0;
                    if(WIFSIGNALED(status))
                        signo = WTERMSIG(status);
                    else
                        signo = WSTOPSIG(status);

                    switch(signo){
                        //TLE
                        case SIGXCPU: case SIGKILL: case SIGPROF:
                            result.setResult(Result_TimeLimitExceed);
                            break;
                        //OLE
                        case SIGXFSZ:
                            break;

                        // RE
                        case SIGSEGV: case SIGABRT:
                        default:
                            result.setResult(Result_RuntimeError);
                            break;
                    }
                    ptrace(PTRACE_KILL, pid);
                    //outputSyscall();
                    fprintf(stderr, "Userexe was killed! The terminated signal is: %s\n", strsignal(signo));
                    if(result.getResult() == Result_TimeLimitExceed){
                        result.updateUsedTime(Time(rused.ru_stime) + Time(rused.ru_utime));
                    }
                    break;
                }

                // Check if the system call is valid.
                /*
                if(!isValidSyscall(syscall_id)) {
                    clog << "The child process trys to call a limited system call: " << syscall_list[syscall_id] << "\n";
                    ptrace(PTRACE_KILL, pid);
                    result = Result_DangerouCode;
                    break;
                }
                 */

                // Check if the child process is MLE.
                result.updateUsedMemory(getMemory(pid));
                if(result.memoryExceed(option.getMemoryLimit())) {
                    ptrace(PTRACE_KILL, pid);
                    result.setResult(Result_MemoryLimitExceed);
                    break;
                }

                //

                if(result.getResult() == Result_Running || result.getResult() == Result_PresentationError)
                    caseTime = Time(rused.ru_stime) + Time(rused.ru_utime);
                if(result.getUsedTime() + caseTime > option.getTimeLimit()) {
                    ptrace(PTRACE_KILL, pid);
                    //timeUsed = timeLimit;
                    result.updateUsedTime(caseTime);
                    result.setResult(Result_TimeLimitExceed);
                    break;
                }


                // Continue the child process until next time it calls a system call.
                ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
            }

            //
            if(result.getResult() == Result_Running || result.getResult() == Result_PresentationError)
                result.updateUsedTime(Time(rused.ru_stime) + Time(rused.ru_utime));
            if(result.timeExceed(option.getTimeLimit())) {
                result.setResult(Result_TimeLimitExceed);
                //timeUsed = timeLimit;
            }

        }
    }

    if(pid > 0) {
        if(result.getResult() == Result_Running) result.setResult(Result_Accepted);
        result.printResult();
    }

    //closedir(dir);

    return 0;
}

unsigned long Executor::getMemory(pid_t pid) {
    char buffer[256];
    char err[1024];
    sprintf(buffer, "/proc/%d/status", pid);
    FILE* fp = fopen(buffer, "r");
    if (fp == NULL) {
        sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), buffer);
        logger.logSysErr(err);
        exit(1);
    }
    unsigned long vmPeak = 0, vmSize = 0, vmExe = 0, vmLib = 0, vmStack = 0;
    while (fgets(buffer, 32, fp)) {
        if (!strncmp(buffer, "VmPeak:", 7)) {
            sscanf(buffer + 7, "%lu", &vmPeak);
        } else if (!strncmp(buffer, "VmSize:", 7)) {
            sscanf(buffer + 7, "%lu", &vmSize);
        } else if (!strncmp(buffer, "VmExe:", 6)) {
            sscanf(buffer + 6, "%lu", &vmExe);
        } else if (!strncmp(buffer, "VmLib:", 6)) {
            sscanf(buffer + 6, "%lu", &vmLib);
        } else if (!strncmp(buffer, "VmStk:", 6)) {
            sscanf(buffer + 6, "%lu", &vmStack);
        }
    }
    fclose(fp);
    if (vmPeak) {
        vmSize = vmPeak;
    }
    return vmSize - vmExe - vmLib - vmStack;
}

void Executor::setLimit() {
    char err[1024];
    rlimit lim;
    //时间限制
    lim.rlim_cur = (option.getTimeLimit() + 999) / 1000;
    lim.rlim_max = lim.rlim_cur;
    if(setrlimit(RLIMIT_CPU, &lim) < 0) {
        sprintf(err, "%s:%d: error: %s", __FILE__, __LINE__, strerror(errno));
        logger.logSysErr(err);
        exit(1);
    }
}

void Executor::IORedirect(const char *std_input_file, const char *usr_output_file) {
    char err[1024];
    if(freopen(std_input_file, "r", stdin) == NULL) {
        sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), std_input_file);
        logger.logSysErr(err);
        exit(1);
    }
    if(freopen(usr_output_file, "w", stdout) == NULL) {
        sprintf(err, "%s:%d: error: %s(file: %s)", __FILE__, __LINE__, strerror(errno), usr_output_file);
        logger.logSysErr(err);
        exit(1);
    }
}
