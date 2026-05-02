#include "tick_generator.h"
#include <random>
#include <chrono>

TickGenerator::TickGenerator(size_t num_ticks) {
    ticks.reserve(num_ticks);

    std::mt19937 rng(42);

    std::uniform_real_distribution<double> mid_move(-0.02, 0.02);
    std::uniform_real_distribution<double> spread_change(-0.005, 0.005);
    std::uniform_int_distribution<int> qty_dist(1, 100);
    std::uniform_int_distribution<int> event_pick(0, 9);

    double mid = 100.0;
    double spread = 0.02;

    int current_levels = 0;  // 🔥 move inside constructor

    for (size_t i = 0; i < num_ticks; ++i) {
        mid += mid_move(rng);

        spread += spread_change(rng);
        if (spread < 0.01) spread = 0.01;
        if (spread > 0.05) spread = 0.05;

        double bid = mid - spread / 2.0;
        double ask = mid + spread / 2.0;

        int qty1 = qty_dist(rng);
        int qty2 = qty_dist(rng);

        // 🔥 EVENT LOGIC (INSIDE LOOP)
        int e1 = event_pick(rng);
        EventType type1;

        if (e1 < 7) type1 = EventType::ADD;
        else if (e1 < 9) type1 = EventType::MODIFY;
        else type1 = EventType::CANCEL;

        if (type1 == EventType::ADD) current_levels++;
        else if (current_levels > 0) current_levels--;

        if (type1 != EventType::ADD && current_levels < 2) {
            type1 = EventType::ADD;
        }

        int e2 = event_pick(rng);
        EventType type2;

        if (e2 < 7) type2 = EventType::ADD;
        else if (e2 < 9) type2 = EventType::MODIFY;
        else type2 = EventType::CANCEL;

        if (type2 == EventType::ADD) current_levels++;
        else if (current_levels > 0) current_levels--;

        if (type2 != EventType::ADD && current_levels < 2) {
            type2 = EventType::ADD;
        }

        // BID tick
        Tick bid_tick;
        bid_tick.timestamp = 0;
        bid_tick.price = bid;
        bid_tick.quantity = qty1;
        bid_tick.side = Side::BID;
        bid_tick.type = type1;

        // ASK tick
        Tick ask_tick;
        ask_tick.timestamp = 0;
        ask_tick.price = ask;
        ask_tick.quantity = qty2;
        ask_tick.side = Side::ASK;
        ask_tick.type = type2;

        ticks.push_back(bid_tick);
        ticks.push_back(ask_tick);
    }
}

std::vector<Tick>& TickGenerator::get_ticks() {
    return ticks;
}