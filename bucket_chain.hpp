#pragma once

#include "bucket.hpp"
#include "lock.hpp"

template <typename Value>
class BucketChain {
   public:
    BucketChain() : head_(new Bucket<Value>) {}

    ~BucketChain() { clear(); }

    Value search(Key key) {
        rw_lock.r_lock();
        int num_traversals = 1;
        Bucket<Value>* bucket = head_;
        while (bucket && !bucket->is_empty() && bucket->get_key() != key) {
            bucket = bucket->get_next();
            num_traversals++;
        }
        if (!bucket) {
            rw_lock.r_unlock();
            return nullptr;
        } else if (bucket->is_empty()) {
            rw_lock.r_unlock();
            return nullptr;
        } else {
            rw_lock.r_unlock();
            return bucket->get_val();
        }
    }

    void insert(Key key, Value val) {
        rw_lock.w_lock();

        Bucket<Value>* bucket = head_;
        if (!bucket) {
            head_ = new Bucket(key, val);
            rw_lock.w_unlock();
            return;
        }
        if (bucket->is_empty()) {
            bucket->set_val(key, val);
            rw_lock.w_unlock();
            return;
        }
        while (bucket->get_next()) {
            bucket = bucket->get_next();
            if (bucket->is_empty()) {
                bucket->set_val(key, val);
                rw_lock.w_unlock();
                return;
            }
        }
        bucket->set_next(new Bucket(key, val));
        rw_lock.w_unlock();
    }

    void del(Key key) {
        rw_lock.w_lock();
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
            rw_lock.w_unlock();
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
            rw_lock.w_unlock();
            return;
        } else {
            rw_lock.w_unlock();
            return;
        }
    }

    void clear() {
        rw_lock.w_lock();
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
        rw_lock.w_unlock();
    }

   private:
    Bucket<Value>* head_;
    mutable ReadersWriterLock rw_lock;
};