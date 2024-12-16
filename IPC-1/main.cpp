#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <cstdlib>


void signal_handler(int sig, siginfo_t *info, void *context)
{
    if (sig == SIGUSR1)
    {
        pid_t sender_pid = info->si_pid;
        uid_t sender_uid = info->si_uid;
        struct passwd *pw = getpwuid(sender_uid);
        const char *sender_user = pw ? pw->pw_name : "No such user";
        printf("Received a SIGUSR1 signal from process [%d] executed by [%d] ([%s]). \n", sender_pid, sender_uid, sender_user);
       
        ucontext_t *uc = (ucontext_t*)context;
        printf("State of the context: RIP = [%lld], RAX = [%lld], RBX = [%lld]. \n", uc->uc_mcontext.gregs[REG_RIP], uc->uc_mcontext.gregs[REG_RAX], uc->uc_mcontext.gregs[REG_RBX]);
    }
}

int main()
{
   
    pid_t pid = getpid();
    std::cout << "PID of current process: " << pid << std::endl;
  
    struct sigaction sa;
    sa.sa_sigaction = signal_handler; 
    sa.sa_flags = SA_SIGINFO;  
    if (sigaction(SIGUSR1, &sa, nullptr) == -1)
    {
        std::cout << "Error signal SIGUSR1" << std::endl;
        exit(errno);
    }
    while (true)
    {
        std::cout << "Programm is sleeping... PID: " << pid << std::endl;
        sleep(10); 
    }
    return 0;
}
