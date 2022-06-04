#pragma once

#include <shared_mutex>

#include "bucket.hpp"

template <typename Value>
class BucketChain {
   public:
    BucketChain() : head_(new Bucket<Value>) {}

    ~BucketChain() { clear(); }

    Value search(Key key) {
        std::shared_lock lock(mutex_);
        int num_traversals = 1;
        Bucket<Value>* bucket = head_;
        while (bucket && !bucket->is_empty() && bucket->get_key() != key) {
            bucket = bucket->get_next();
            num_traversals++;
        }
        if (!bucket)
            return nullptr;
        else if (bucket->is_empty())
            return nullptr;
        else
            return bucket->get_val();
    }

    void insert(Key key, Value val) {
        std::unique_lock lock(mutex_);

        Bucket<Value>* bucket = head_;
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

    void del(Key key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        Bucket<Value>* bucket = head_;

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
            Bucket<Value>* del_bucket = bucket->get_next();
            bucket->set_next(del_bucket->get_next());
            del_bucket->set_next(nullptr);
            delete del_bucket;
            return;
        } else {
            return;
        }
    }

    void clear() {
        std::unique_lock lock(mutex_);

        Bucket<Value>* bucket = head_;
        Bucket<Value>* prev;
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

   private:
    Bucket<Value>* head_;
    mutable std::shared_mutex mutex_;
};