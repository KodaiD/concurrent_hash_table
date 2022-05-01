#pragma once

#include "bucket_chain.hpp"

class HashTable {
   public:
    HashTable(int);
    ~HashTable();
    std::pair<Value, int> search(Key);
    void insert(Key, Value);
    void del(Key);

   private:
    int hash(Key);
    BucketChain* table_;
    const int table_size_;
};