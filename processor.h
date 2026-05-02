#pragma once
#include "order_book.h"
#include "latency_tracker.h"
#include "lockfree_queue.h"
#include "strategy.h"
#include <chrono>
#include <cstdio>

class Processor {
public:
    Processor(SPSCQueue<Tick>& q)
        : queue(q) {}

    void run(size_t max_ticks) {

        Tick tick;
        size_t counter = 0;

        uint64_t start_time = now();

        while (running && counter < max_ticks) {
            if (queue.pop(tick)) {

                uint64_t start = now();

                book.update(tick);

                // 🔥 STRATEGY HOOK (VERY IMPORTANT)
                strategy.on_tick(tick, book);

                uint64_t end = now();
                tracker.add(end - start);

                counter++;
            }
        }

        uint64_t end_time = now();
        double seconds = (end_time - start_time) / 1e9;

        printf("Throughput: %.2f ticks/sec\n", counter / seconds);

        // 🔥 PRINT STRATEGY RESULT
        strategy.report();
    }

    void stop() { running = false; }

    void report() { tracker.report(); }

private:
    uint64_t now() {
        return std::chrono::high_resolution_clock::now()
            .time_since_epoch().count();
    }

    SPSCQueue<Tick>& queue;
    OrderBook book;
    LatencyTracker tracker;
    Strategy strategy;   // 🔥 NEW
    bool running = true;
};