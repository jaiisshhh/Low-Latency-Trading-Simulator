#pragma once
#include <vector>
#include <algorithm>
#include <cstdio>

class LatencyTracker {
public:
    void add(uint64_t latency) {
        latencies.push_back(latency);
    }

    void report() {
        if (latencies.empty()) return;

        std::sort(latencies.begin(), latencies.end());

        size_t n = latencies.size();

        auto p50  = latencies[n * 50 / 100];
        auto p99  = latencies[n * 99 / 100];
        auto p999 = latencies[n * 999 / 1000];

        printf("Latency Stats:\n");
        printf("P50  : %llu ns (%.3f µs)\n", (unsigned long long)p50,  p50 / 1000.0);
        printf("P99  : %llu ns (%.3f µs)\n", (unsigned long long)p99,  p99 / 1000.0);
        printf("P999 : %llu ns (%.3f µs)\n", (unsigned long long)p999, p999 / 1000.0);
    }

private:
    std::vector<uint64_t> latencies;
};