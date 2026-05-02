#pragma once
#include "types.h"
#include <vector>

class TickGenerator {
public:
    TickGenerator(size_t num_ticks);
    std::vector<Tick>& get_ticks();

private:
    std::vector<Tick> ticks;
};