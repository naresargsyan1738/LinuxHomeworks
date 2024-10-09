#include <iostream>
#include <unistd.h> 
#include <fcntl.h>  
#include <cerrno> 
#include <string>

void initialize(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Error";
        exit(errno);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        std::cerr << "ERROR,file can't be opened\n";
        exit(errno);
    }

    if (dup2(fd,0) < 0) {
        std::cerr << "ERROR, can't be redirected stdin\n";
        close(fd);
        exit(errno);
    }

    close(fd);
}

int main(int argc, char** argv)
{
    initialize(argc, argv);
    std::string input;
    std::cin>> input;
    
    std::string reversed(input.rbegin(), input.rend());
    std::cout <<reversed << std::endl;

    return 0;
};

