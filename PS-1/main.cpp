#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <ctime>    
#include <cstring>  

void do_command(char** argv) {
    std::clock_t start_time = std::clock();

    pid_t child = fork();

    if (child == 0) {
        execvp(argv[0],argv);
        std::cerr << "Exec error " << strerror(errno) << std::endl;
        exit(1);
    } 
    else if (child < 0) {
        std::cerr << "Fork error " << strerror(errno) << std::endl;
        exit(1);
    } 
    else {
        int status;
        waitpid(child, &status, 0);

        std::clock_t end_time = std::clock();
        double duration = double(end_time - start_time) / CLOCKS_PER_SEC;

        if (WIFEXITED(status)) {
            std::cout <<"Command completed with exit code:" << WEXITSTATUS(status) << std::endl;
        } 
        else if (WIFSIGNALED(status)) {
            std::cout << "Command completed with exit code:"<<WTERMSIG(status) << std::endl;
        }
        std::cout << "Command took " << duration << " seconds." << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Invalid arguments" << std::endl;
        return 1;
    }

    do_command(argv + 1);

    return 0;
}
