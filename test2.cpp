#include <cassert>
#include <chrono>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

#include "hash_table.hpp"

#define DATA_SIZE 10000000
#define TABLE_SIZE 10000000
#define NUM_THREADS 16
#define NUM_OPS 5

struct Record {
    Record(int key, int val) : key(key), val(val) {}
    int key;
    int val;
};

void insert_worker(HashTable<Record*>& ht, int i) {
    for (int j = i * DATA_SIZE / NUM_THREADS;
         j < (i + 1) * DATA_SIZE / NUM_THREADS; j++) {
        ht.insert(j, new Record(j, j));
    }
}

void search_worker(HashTable<Record*>& ht, int i) {
    for (int j = i * DATA_SIZE / NUM_THREADS;
         j < (i + 1) * DATA_SIZE / NUM_THREADS; j++) {
        assert(ht.search(j)->val == j);
    }
}

void delete_worker(HashTable<Record*>& ht, int i) {
    for (int j = i * DATA_SIZE / NUM_THREADS;
         j < (i + 1) * DATA_SIZE / NUM_THREADS; j++) {
        ht.del(j);
    }
}

void print_ave(std::string s, std::vector<int>& v) {
    int result = std::accumulate(v.begin(), v.end(), 0.0) / NUM_OPS;
    std::cout << s << "[average]: " << result << " msec \n";
}

int main() {
    std::vector<int> insert_result;
    std::vector<int> search_result;
    std::vector<int> delete_result;

    std::cout << "omp num threads: " << NUM_THREADS << "\n\n";

    for (int n = 0; n < NUM_OPS; n++) {
        HashTable<Record*> ht(TABLE_SIZE);

        // insert

        std::vector<std::thread> insert_workers;
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            insert_workers.emplace_back(insert_worker, std::ref(ht), i);
        }
        for (auto& t : insert_workers) {
            t.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto dur = end - start;
        auto msec =
            std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        insert_result.emplace_back(msec);
        std::cout << "insert: " << msec << " msec \n";

        // search

        std::vector<std::thread> search_workers;
        start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            search_workers.emplace_back(search_worker, std::ref(ht), i);
        }
        for (auto& t : search_workers) {
            t.join();
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

        std::vector<std::thread> delete_workers;
        start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < NUM_THREADS; i++) {
            delete_workers.emplace_back(delete_worker, std::ref(ht), i);
        }
        for (auto& t : delete_workers) {
            t.join();
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

    std::cout << "\n===== PASS =====\n\n";

    return 0;
}