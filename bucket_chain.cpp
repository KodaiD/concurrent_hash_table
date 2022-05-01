#include "bucket_chain.hpp"

BucketChain::BucketChain() : head(nullptr) {}

BucketChain::~BucketChain() { clear(); }

std::pair<Value, int> BucketChain::search(Key key) {
    std::shared_lock lock(mutex);
    int num_traversals = 1;
    Bucket* bucket = head;
    while (bucket && bucket->get_key() != key) {
        bucket = bucket->get_next();
        num_traversals++;
    }
    if (bucket)
        return std::make_pair(bucket->get_val(), num_traversals);
    else
        return std::make_pair(-1, -1);
}

void BucketChain::insert(Key key, Value val) {
    Bucket* new_bucket = new Bucket(key, val);
    std::unique_lock lock(mutex);

    Bucket* bucket = head;
    if (!bucket) {
        head = new_bucket;
    } else {
        while (bucket->get_next()) {
            bucket = bucket->get_next();
        }
        bucket->set_next(new_bucket);
    }
}

void BucketChain::del(Key key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    Bucket* bucket = head;

    // no data in the chain
    if (!bucket) return;

    // found at the head of the chain
    if (head->get_key() == key) {
        if (!head->get_next()) {
            head = nullptr;
            delete bucket;
        } else {
            head = head->get_next();
            bucket->set_next(nullptr);
            delete bucket;
        }
        return;
    }

    // chain traversal
    while (bucket->get_next() && bucket->get_next()->get_key() != key) {
        bucket = bucket->get_next();
    }
    if (bucket->get_next()) {
        Bucket* del_bucket = bucket->get_next();
        bucket->set_next(del_bucket->get_next());
        del_bucket->set_next(nullptr);
        delete del_bucket;
        return;
    } else {
        return;
    }
}

void BucketChain::clear() {
    std::unique_lock lock(mutex);

    Bucket* bucket = head;
    Bucket* prev;
    if (bucket) {
        while (bucket->get_next()) {
            prev = bucket;
            bucket = bucket->get_next();
            delete prev;
        }
        delete bucket;
    }
    head = nullptr;
}