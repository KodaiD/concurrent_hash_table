#pragma once

#include "definition.hpp"

class Bucket {
   public:
    Bucket();
    Bucket(Key key, Value value);
    ~Bucket();
    Key get_key();
    Value get_val();
    void set_val(Key key, Value value);
    Bucket* get_next();
    void set_next(Bucket* bucket);

   private:
    Bucket* next;
    Key key;
    Value val;
};