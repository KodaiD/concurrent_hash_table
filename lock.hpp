#include <atomic>

class ReadersWriterLock {
   public:
    ReadersWriterLock() {}

    void init() { counter.store(0, std::memory_order_release); }

    bool r_lock() {
        int expected;
        while (1) {
            expected = counter.load(std::memory_order_acquire);
            if (expected >= 0 &&
                counter.compare_exchange_strong(expected, expected + 1,
                                                std::memory_order_acq_rel,
                                                std::memory_order_acquire))
                return true;
        }
        return false;
    }

    bool w_lock() {
        int expected;
        while (1) {
            expected = counter.load(std::memory_order_acquire);
            if (expected == 0 && counter.compare_exchange_strong(
                                     expected, -1, std::memory_order_acq_rel,
                                     std::memory_order_acquire))
                return true;
        }
        return false;
    }

    void r_unlock() { counter.fetch_add(-1); }

    void w_unlock() { counter.fetch_add(1); }

   private:
    std::atomic<int> counter = 0;
};