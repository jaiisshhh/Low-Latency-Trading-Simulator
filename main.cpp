#include "data_loader.h"
#include "lockfree_queue.h"
#include "processor.h"

#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>

int main()
{
    std::cout << "Loading data...\n";

    auto ticks = DataLoader::load("Market_data/BTCUSDT-trades-2026-05-01.csv");

    std::cout << "Loaded ticks: " << ticks.size() << "\n";

    SPSCQueue<Tick> queue(1 << 20);
    Processor processor(queue);

    std::atomic<bool> done(false);

    std::thread producer([&]()
                         {
        for (auto& t : ticks) {

            // 🔥 convert ms → ns (VERY IMPORTANT)
            t.timestamp *= 1000000;

            while (!queue.push(t)) {
                // spin
            }
        } });

    std::thread consumer([&]()
                         {
        processor.run(ticks.size());
        done = true; });

    producer.join();
    consumer.join();

    processor.report();

    return 0;
}