#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>

int main() {
    std::string source_file, destination_file;
    off_t source_offset, dest_offset, bytes_to_copy;

    std::cout << "Enter the name of the source file: ";
    std::cin >> source_file;

    std::cout << "Enter the name of the destination file: ";
    std::cin >> destination_file;

    std::cout << "Enter the initial offset in the source file (in bytes): ";
    std::cin >> source_offset;

    std::cout << "Enter the initial offset in the destination file (in bytes): ";
    std::cin >> dest_offset;

    std::cout << "Enter the number of bytes to copy: ";
    std::cin >> bytes_to_copy;

    int fd1 = open(source_file.c_str(), O_RDONLY);
    if (fd1 < 0) {
        std::cerr << "Error opening the file "<< std::endl;
        exit(EXIT_FAILURE);
    }
    
    int fd2 = open(destination_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0) {
        std::cerr << "Error opening the file "<< std::endl;
 close(fd1);
        exit(EXIT_FAILURE);
    }
    lseek(fd1, source_offset, SEEK_SET);
    lseek(fd2, dest_offset, SEEK_SET);

    char buffer[4096];
      ssize_t total = 0;
    while (total < bytes_to_copy) {
        ssize_t bytes_read = read(fd1, buffer, sizeof(buffer));
        if (bytes_read <= 0) break;
        ssize_t bytes_written = write(fd2, buffer, std::min(bytes_read, bytes_to_copy - total));
        if (bytes_written < 0) break;
        total += bytes_written;
    }

    std::cout << "Successfully copied " << total << " bytes." << std::endl;
    close(fd1);
    close(fd2);
    return 0;

}
