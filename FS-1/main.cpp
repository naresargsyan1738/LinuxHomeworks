#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
 
int main(int argc, char **argv){
	 if (argc != 2) {
        std::cerr << argv[0] << std::endl;
        return 1;
    } 
 int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        std::cerr << errno << std::endl;
        return errno;
    }
    char buffer[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        std::cout.write(buffer, bytes_read);
    }

    if (bytes_read == -1) {
        std::cerr << errno << std::endl;
        close(fd);
        return errno;
    }

    if (close(fd) == -1) {
        std::cerr << errno << std::endl;
        return errno;
    }

   
    return 0;
}


