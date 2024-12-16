#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <cstdlib>
#include <ctime>

struct ThreadData
{
    const std::vector<int>* arr;
    long long* result;
    size_t start;
    size_t end;

    // Constructor to initialize the values
    ThreadData(const std::vector<int>* arr, long long* result, size_t start, size_t end)
        : arr(arr), result(result), start(start), end(end) {}
};

void* sum_part(void* arg)
{
    ThreadData* data = static_cast<ThreadData*>(arg);
    const auto& arr = *data->arr;
    long long* result = data->result;
    size_t start = data->start;
    size_t end = data->end;

    *result = 0;
    for (size_t i = start; i < end; ++i)
    {
        *result += arr[i];
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 3)
    {
        std::cout << "Invalid arguments" << std::endl;
        exit(errno);
    }
    size_t N = std::stoull(argv[1]);
    size_t M = std::stoull(argv[2]);

    std::vector<int> arr(N);
    for (size_t i = 0; i < N; ++i)
    {
        arr[i] = rand() % 100;  
    }

    auto start_time = std::chrono::high_resolution_clock::now(); 
    long long regular_sum = 0;
    for (size_t i = 0; i < N; ++i)
    {
        regular_sum += arr[i];
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_regular = std::chrono::duration<double>(end_time - start_time).count();

    std::cout << "Time taken without threads: " << duration_regular << " seconds\n";

    std::vector<pthread_t> threads(M);   
    std::vector<long long> results(M, 0); 

    size_t part_size = N / M;  
    start_time = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < M; ++i)
    {
        size_t start_index = i * part_size;
        size_t end_index = (i == M - 1) ? N : (i + 1) * part_size;  
        
        // Create ThreadData with the constructor
        ThreadData* data = new ThreadData(&arr, &results[i], start_index, end_index);
        pthread_create(&threads[i], nullptr, sum_part, data);
    }
    for (size_t i = 0; i < M; ++i)
    {
        pthread_join(threads[i], nullptr);
    }
    long long threaded_sum = 0;
    for (size_t i = 0; i < M; ++i)
    {
        threaded_sum += results[i];
    }

    end_time = std::chrono::high_resolution_clock::now();
    auto duration_threaded = std::chrono::duration<double>(end_time - start_time).count();

    std::cout << "Time taken with threads: " << duration_threaded << " seconds\n";

    return 0;
}
