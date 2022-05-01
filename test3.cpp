#include <omp.h>
#include <unistd.h>

#include <cassert>
#include <chrono>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include "hash_table.hpp"

#define DATA_SIZE 10000000
#define TABLE_SIZE 10000000
#define NUM_THREADS 1
#define NUM_OPS 5

void print_ave(std::string s, std::vector<int>& v) {
    int result = std::accumulate(v.begin(), v.end(), 0.0) / NUM_OPS;
    std::cout << s << "[average]: " << result << " msec \n";
}

int main() {
    std::vector<int> insert_result;
    std::vector<int> search_result;
    std::vector<int> delete_result;

    for (int n = 0; n < NUM_OPS; n++) {
        HashTable ht(TABLE_SIZE);

        // insert

        auto start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
        for (int i = 0; i < DATA_SIZE; i++) {
            ht.insert(i, i);
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;
        auto msec =
            std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        insert_result.emplace_back(msec);
        std::cout << "insert: " << msec << " msec \n";

        // search

        start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
        for (int i = 0; i < DATA_SIZE; i++) {
            assert(ht.search(i).first == i);
        }

        end = std::chrono::high_resolution_clock::now();
        dur = end - start;
        msec =
            std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        search_result.emplace_back(msec);
        std::cout << "search: " << msec << " msec \n";

        // check the number of traversal

        // std::cout << ht.search(99).second << std::endl;
        // std::cout << ht.search(9999).second << std::endl;

        // delete

        start = std::chrono::high_resolution_clock::now();

#pragma omp parallel for
        for (int i = 0; i < DATA_SIZE; i++) {
            ht.del(i);
        }

        end = std::chrono::high_resolution_clock::now();
        dur = end - start;
        msec =
            std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        delete_result.emplace_back(msec);
        std::cout << "delete: " << msec << " msec \n";

        std::cout << std::endl;
    }

    print_ave("insert", insert_result);
    print_ave("search", search_result);
    print_ave("delete", delete_result);

    std::cout << "===== PASS =====" << std::endl;

    return 0;
}