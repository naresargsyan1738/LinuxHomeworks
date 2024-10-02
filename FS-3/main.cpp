#include <cerrno>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Inv args" << std::endl;
        return 1;
    }

    int file_desc = open(argv[1], O_RDWR);
    if (file_desc == -1) {
        std::cerr << "open" << std::endl;
        return errno;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(file_desc, buffer, sizeof(buffer))) > 0) {
        lseek(file_desc, -bytes_read, SEEK_CUR);
        if (write(file_desc, "\0", bytes_read) == -1) {
            std::cerr << "write" << std::endl;
            close(file_desc);
            return errno;
        }
    }

    if (bytes_read == -1) {
        std::cerr << "read" << std::endl;
    }

    close(file_desc);

    if (unlink(argv[1]) == -1) {
        std::cerr << "delete" << std::endl;
        return errno;
    }
    return 0;
}

