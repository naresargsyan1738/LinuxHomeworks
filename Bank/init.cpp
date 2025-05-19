#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <sys/stat.h>
#include <cstring>
#include <cerrno>

class Cell {
public:
    int cur_b;
    int min;
    int max;
    bool frozen;
};

class TBank {
public:
    TBank(size_t n) : size(n) {
        for (size_t i = 0; i < n; ++i) {
            cells[i].cur_b = 0;
            cells[i].min = 0;
            cells[i].max = 1000;
            cells[i].frozen = false;
        }
    }

    void print() {
        for (size_t i = 0; i < size; ++i) {
            std::cout << "Account " << i
                      << ": balance=" << cells[i].cur_b
                      << ", min=" << cells[i].min
                      << ", max=" << cells[i].max
                      << ", frozen=" << (cells[i].frozen ? "true" : "false")
                      << "\n";
        }
    }

private:
    size_t size;
    Cell cells[1];
};

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;

    size_t N = std::atoi(argv[1]);
    const char* shm_name = "/TBank";
    const char* sem_name = "/TBankSem";

    size_t size = sizeof(TBank) + (N - 1) * sizeof(Cell);

    int fd = shm_open(shm_name, O_CREAT | O_RDWR | O_TRUNC, 0666);
    ftruncate(fd, size);
    void* ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t* sem = sem_open(sem_name, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        std::cerr << "sem_open failed: " << strerror(errno) << "\n";
        munmap(ptr, size);
        close(fd);
        shm_unlink(shm_name);
        return 1;
    }

    sem_wait(sem);

    TBank* bank = new (ptr) TBank(N);
    bank->print();

    sem_post(sem);

    close(fd);

    sem_close(sem);

    return 0;
}

