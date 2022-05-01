#include "bucket.hpp"

Bucket::Bucket() : next(nullptr) {}

Bucket::Bucket(Key key, Value value) : next(nullptr), key(key), val(value) {}

Bucket::~Bucket() { next = nullptr; }

Key Bucket::get_key() { return key; }

Value Bucket::get_val() { return val; }

void Bucket::set_val(Key key, Value val) {
    key = key;
    val = val;
}

Bucket* Bucket::get_next() { return next; }

void Bucket::set_next(Bucket* bucket) { next = bucket; }