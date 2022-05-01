#include "bucket_chain.hpp"

BucketChain::BucketChain() : head_(new Bucket) {}

BucketChain::~BucketChain() { clear(); }

std::pair<Value, int> BucketChain::search(Key key) {
    std::shared_lock lock(mutex_);
    int num_traversals = 1;
    Bucket* bucket = head_;
    while (bucket && !bucket->is_empty() && bucket->get_key() != key) {
        bucket = bucket->get_next();
        num_traversals++;
    }
    if (!bucket)
        return std::make_pair(-1, num_traversals);
    else if (bucket->is_empty())
        return std::make_pair(-1, num_traversals);
    else
        return std::make_pair(bucket->get_val(), num_traversals);
}

void BucketChain::insert(Key key, Value val) {
    std::unique_lock lock(mutex_);

    Bucket* bucket = head_;
    if (!bucket) {
        head_ = new Bucket(key, val);
        return;
    }
    if (bucket->is_empty()) {
        bucket->set_val(key, val);
        return;
    }
    while (bucket->get_next()) {
        bucket = bucket->get_next();
        if (bucket->is_empty()) {
            bucket->set_val(key, val);
            return;
        }
    }
    bucket->set_next(new Bucket(key, val));
}

void BucketChain::del(Key key) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    Bucket* bucket = head_;

    // no data in the chain
    if (!bucket) return;

    // found at the head of the chain
    if (head_->get_key() == key) {
        if (!head_->get_next()) {
            head_ = nullptr;
            delete bucket;
        } else {
            head_ = head_->get_next();
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
    std::unique_lock lock(mutex_);

    Bucket* bucket = head_;
    Bucket* prev;
    if (bucket) {
        while (bucket->get_next()) {
            prev = bucket;
            bucket = bucket->get_next();
            delete prev;
        }
        delete bucket;
    }
    head_ = nullptr;
}