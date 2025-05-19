#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <semaphore.h>
#include <string>
#include <sstream>
#include <sys/stat.h>

class Cell {
public:
    int cur_b;
    int min;
    int max;
    bool frozen;
};

class TBank {
public:
    size_t size;
    Cell cells[1];

    void print_account(size_t i) {
        if (i >= size) {
            std::cout << "Invalid account number\n";
            return;
        }
        Cell& c = cells[i];
        std::cout << "Account " << i << ": balance=" << c.cur_b
                  << ", min=" << c.min << ", max=" << c.max
                  << ", frozen=" << (c.frozen ? "true" : "false") << "\n";
    }

    bool transfer(size_t a, size_t b, int amount) {
        if (a >= size || b >= size || amount <= 0) return false;
        Cell& from = cells[a];
        Cell& to = cells[b];
        if (from.frozen || to.frozen) return false;
        if (from.cur_b - amount < from.min || to.cur_b + amount > to.max)
            return false;
        from.cur_b -= amount;
        to.cur_b += amount;
        return true;
    }

    bool add_all(int delta) {
        for (size_t i = 0; i < size; ++i) {
            if (cells[i].frozen) return false;
            if (cells[i].cur_b + delta < cells[i].min || cells[i].cur_b + delta > cells[i].max)
                return false;
        }
        for (size_t i = 0; i < size; ++i) {
            cells[i].cur_b += delta;
        }
        return true;
    }

    bool set_limit(size_t a, bool is_min, int value) {
        if (a >= size) return false;
        if (is_min)
            cells[a].min = value;
        else
            cells[a].max = value;
        return true;
    }

    bool freeze(size_t a, bool yes) {
        if (a >= size) return false;
        cells[a].frozen = yes;
        return true;
    }
};

int main() {
    const char* shm_name = "/TBank";
    const char* sem_name = "/TBankSem";

    int fd = shm_open(shm_name, O_RDWR, 0666);
    if (fd < 0) {
        std::cerr << "Cannot open shared memory.\n";
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        std::cerr << "fstat failed\n";
        close(fd);
        return 1;
    }
    size_t map_size = sb.st_size;

    void* ptr = mmap(nullptr, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        std::cerr << "mmap failed\n";
        close(fd);
        return 1;
    }

    TBank* bank = reinterpret_cast<TBank*>(ptr);

    sem_t* sem = sem_open(sem_name, 0);
    if (sem == SEM_FAILED) {
        std::cerr << "Cannot open semaphore\n";
        munmap(ptr, map_size);
        close(fd);
        return 1;
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        sem_wait(sem);

        if (cmd == "print") {
            size_t a;
            iss >> a;
            bank->print_account(a);
        } else if (cmd == "transfer") {
            size_t a, b;
            int x;
            iss >> a >> b >> x;
            if (bank->transfer(a, b, x))
                std::cout << "Transfer successful\n";
            else
                std::cout << "Transfer failed\n";
        } else if (cmd == "add") {
            int x;
            iss >> x;
            if (bank->add_all(x))
                std::cout << "Add successful\n";
            else
                std::cout << "Add failed\n";
        } else if (cmd == "freeze") {
            size_t a;
            iss >> a;
            if (bank->freeze(a, true))
                std::cout << "Account frozen\n";
            else
                std::cout << "Operation failed\n";
        } else if (cmd == "unfreeze") {
            size_t a;
            iss >> a;
            if (bank->freeze(a, false))
                std::cout << "Account unfrozen\n";
            else
                std::cout << "Operation failed\n";
        } else if (cmd == "setmin") {
            size_t a;
            int x;
            iss >> a >> x;
            if (bank->set_limit(a, true, x))
                std::cout << "Min set\n";
            else
                std::cout << "Failed\n";
        } else if (cmd == "setmax") {
            size_t a;
            int x;
            iss >> a >> x;
            if (bank->set_limit(a, false, x))
                std::cout << "Max set\n";
            else
                std::cout << "Failed\n";
        } else {
            std::cout << "Unknown command\n";
        }

        sem_post(sem);
    }

    munmap(ptr, map_size);
    close(fd);
    sem_close(sem);
    return 0;
}

