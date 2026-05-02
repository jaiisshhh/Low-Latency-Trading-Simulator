#pragma once
#include <atomic>
#include <vector>

template<typename T>
class SPSCQueue {
public:
    SPSCQueue(size_t size)
        : buffer(size), head(0), tail(0), capacity(size) {}

    bool push(const T& item) {
        auto next = (head + 1) % capacity;
        if (next == tail) return false; // full
        buffer[head] = item;
        head = next;
        return true;
    }

    bool pop(T& item) {
        if (tail == head) return false; // empty
        item = buffer[tail];
        tail = (tail + 1) % capacity;
        return true;
    }

private:
    std::vector<T> buffer;
    std::atomic<size_t> head, tail;
    size_t capacity;
};
