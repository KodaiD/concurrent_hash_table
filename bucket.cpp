#include "bucket.hpp"

Bucket::Bucket() : next_(nullptr), empty_(true) {}

Bucket::Bucket(Key key, Value value)
    : next_(nullptr), key_(key), val_(value), empty_(false) {}

Bucket::~Bucket() { next_ = nullptr; }

Key Bucket::get_key() { return key_; }

Value Bucket::get_val() { return val_; }

void Bucket::set_val(Key key, Value val) {
    key_ = key;
    val_ = val;
    empty_ = false;
}

Bucket* Bucket::get_next() { return next_; }

void Bucket::set_next(Bucket* bucket) { next_ = bucket; }

bool Bucket::is_empty() { return empty_; }