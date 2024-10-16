#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sstream>
#include <cmath>
#include <limits>
 
int main(int argc, char** argv){
 
       if(argc == 1){
        std::cerr << "Missing filename argument" << std::endl;
        exit(1);
    }
 
    const char* filename = argv[1];
 
    int file = open(filename, O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
 
    if(file < 0){
        std::cerr << "Error while opening the file " << filename << ". " << strerror(errno) << std::endl;
        exit(errno);
    }
 
    long long bufferSize = 10485760;
 
    char* buffer = new char[bufferSize];
 
    for(int i = 0; i < bufferSize; ++i){
        buffer[i] = 'x';
    }
 
    for(int i = 2; i < argc; ++i){
        
        bool isData = i % 2 == 0;
 
        const char* type = isData ? "DATA: " : "HOLE: ";
 
        const char* size = argv[i];
 
        long long bytes = 0;
 
          std::stringstream stream(size);

        stream >> bytes;
 
         if(stream.fail()){
            std::cout << type << "Warning: " << "Could not parse the value " << size << " as an integer. Will use 0 instead." << std::endl;
            bytes = 0;
        }
 
        std::cout << type << "Creating area of " << bytes << " bytes..." << std::endl;
 
        if(!isData){
            
            long long segmentHole = 0;

            while(segmentHole < bytes){
 
                long long leftToHole = std::min(bytes - segmentHole, bufferSize);
 
                off_t holeCreated = lseek(file, (off_t) leftToHole, SEEK_END);
 
                if(holeCreated < 0){
                    std::cerr << type << "Could not create a hole with size " << bytes << " because of error " << strerror(errno) << std::endl;
                    break;
                }
                segmentHole += holeCreated;
            }
            continue;
        }
 
        long long segmentWritten = 0;
 
        while(segmentWritten < bytes){
            
            size_t leftToWrite = std::min(bytes - segmentWritten, bufferSize);
 
            ssize_t written = write(file, buffer, leftToWrite);
 
            if(written < 0){
                std::cerr << type << "Error while writing data into the file due to " << strerror(errno) << ". Written bytes for region: " << segmentWritten << std::endl;
                break;
            }

            segmentWritten += written;
        }
    }
 
    int closed = close(file);
 
    delete [] buffer;
    return closed < 0 ? errno : 0;
}
