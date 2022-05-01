#include <cassert>
#include <iostream>

#include "hash_table.hpp"

#define DATA_SIZE 10000000
#define TABLE_SIZE 10000000

int main() {
    HashTable ht(TABLE_SIZE);

    for (int i = 0; i < DATA_SIZE; i++) {
        ht.insert(i, i);
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        assert(ht.search(i).first == i);
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        ht.del(i);
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        assert(ht.search(i).second == -1);
    }

    std::cout << "===== PASS =====" << std::endl;
}