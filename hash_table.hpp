#pragma once

#include "bucket_chain.hpp"

template <typename Value>
class HashTable {
   public:
    HashTable(uint64_t table_size) : table_size_(table_size) {
        table_ = new BucketChain<Value>[table_size];
    }

    ~HashTable() { delete[] table_; }

    Value search(Key key) { return table_[hash(key)].search(key); }
    void insert(Key key, Value val) {
        return table_[hash(key)].insert(key, val);
    }
    void del(Key key) { return table_[hash(key)].del(key); }

   private:
    uint64_t hash(Key key) { return key % table_size_; }
    BucketChain<Value>* table_;
    const uint64_t table_size_;
};