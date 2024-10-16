#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>  // Для strerror

int main(int argc, char** argv){
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <source-file> <destination-file>" << std::endl;
        return 1;
    }

    int source_file = open(argv[1], O_RDONLY);
    if (source_file == -1) {
        std::cerr << "Can't open source file: " << strerror(errno) << std::endl;
        return 1;
    }

    int destination = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (destination == -1) {
        std::cerr << "Can't open destination file: " << strerror(errno) << std::endl;
        close(source_file);
        return 1;
    }

    const int buffer_size = 4096;
    char buffer[buffer_size];
    ssize_t bytes_read;
    int total_bytes = 0, holes_count = 0, data_bytes = 0;

    while ((bytes_read = read(source_file, buffer, buffer_size)) > 0) {
        for (ssize_t i = 0; i < bytes_read; ++i) {
            if (buffer[i] == 0) {
                ++holes_count;
            } else {
                if (write(destination, &buffer[i], 1) == -1) {
                    std::cerr << "Can't write to destination file: " << strerror(errno) << std::endl;
                    close(source_file);
                    close(destination);
                    return 1;
                }
                ++data_bytes;
            }
            ++total_bytes;
        }
    }

    if (bytes_read == -1) {
        std::cerr << "Can't read from source file: " << strerror(errno) << std::endl;
        close(source_file);
        close(destination);
        return 1;
    }

    std::cout << "Successfully copied " << total_bytes << " bytes (data: "
              << data_bytes << ", holes: " << holes_count << ")" << std::endl;

    close(destination);
    close(source_file);

    return 0;
}


