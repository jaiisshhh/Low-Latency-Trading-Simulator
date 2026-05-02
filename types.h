#pragma once
#include <cstdint>

enum class Side { BID, ASK };
enum class EventType { ADD, CANCEL, MODIFY };

struct Tick {
    uint64_t timestamp;
    double price;
    int quantity;
    Side side;
    EventType type;
};