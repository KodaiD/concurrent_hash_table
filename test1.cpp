#include <cassert>
#include <iostream>

#include "hash_table.hpp"

#define DATA_SIZE 10000000
#define TABLE_SIZE 10000000

struct Record {
    Record(int key, int val) : key(key), val(val) {}
    int key;
    int val;
};

int main() {
    HashTable<Record*> ht(TABLE_SIZE);

    for (int i = 0; i < DATA_SIZE; i++) {
        ht.insert(i, new Record(i, i));
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        assert(ht.search(i)->val == i);
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        ht.del(i);
    }

    for (int i = 0; i < DATA_SIZE; i++) {
        assert(ht.search(i) == nullptr);
    }

    std::cout << "\n===== PASS =====\n\n";
}