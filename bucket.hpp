#pragma once

#include "definition.hpp"

template <typename Value>
class Bucket {
   public:
    Bucket() : next_(nullptr), empty_(true) {}
    Bucket(Key key, Value value)
        : next_(nullptr), key_(key), val_(value), empty_(false) {}
    ~Bucket() { next_ = nullptr; }
    Key get_key() { return key_; }
    Value get_val() { return val_; }
    void set_val(Key key, Value val) {
        key_ = key;
        val_ = val;
        empty_ = false;
    }
    Bucket<Value>* get_next() { return next_; }
    void set_next(Bucket* bucket) { next_ = bucket; }
    bool is_empty() { return empty_; }

   private:
    Bucket<Value>* next_;
    Key key_;
    Value val_;
    bool empty_;
};