#pragma once

#include <shared_mutex>
#include <utility>

#include "bucket.hpp"

class BucketChain {
   public:
    BucketChain();
    ~BucketChain();
    BucketChain(const BucketChain& other) = delete;
    BucketChain& operator=(const BucketChain& other) = delete;
    std::pair<Value, int> search(Key key);
    void insert(Key key, Value val);
    void del(Key key);
    void clear();

   private:
    Bucket* head;
    mutable std::shared_mutex mutex;
};