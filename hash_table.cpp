#include "hash_table.hpp"

HashTable::HashTable(int table_size) : table_size_(table_size) {
    table_ = new BucketChain[table_size];
}

HashTable::~HashTable() { delete[] table_; }

std::pair<Value, int> HashTable::search(Key key) {
    return table_[hash(key)].search(key);
}

void HashTable::insert(Key key, Value val) {
    return table_[hash(key)].insert(key, val);
}

void HashTable::del(Key key) { return table_[hash(key)].del(key); }

int HashTable::hash(Key key) { return key % table_size_; }