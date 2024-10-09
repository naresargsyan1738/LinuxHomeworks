#include <iostream>
#include <fcntl.h>  
#include <unistd.h> 

void print_error(const std::string& message) {
    std::cerr << "error: " << message << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_error("no input");
        return 1;
    }
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0) {
        print_error("can't be opened");
        return 1;
    }
    
    int new_fd = dup(fd);
    if (new_fd < 0) {
        print_error("failed to duplicate");
        close(fd);
        return 1;
    }

    if (write(fd, "first line\n", 11) < 0) {
        print_error("can't write first line\n");
        close(fd);
        close(new_fd);
        return 1;
    }
    
    if (write(new_fd, "second line\n", 12) < 0) {
        print_error("can't write second line");
        close(fd);
       return 1;
    }

    close(fd);
    close(new_fd);

    return 0;  
}
