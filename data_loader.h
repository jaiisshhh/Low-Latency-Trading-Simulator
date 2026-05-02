#pragma once
#include "types.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

class DataLoader {
public:
    static std::vector<Tick> load(const std::string& filename) {
        std::vector<Tick> ticks;
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {

            if (line.empty()) continue;  // 🔥 skip empty lines

            std::stringstream ss(line);
            std::string token;

            Tick t;

            try {
                // trade_id (ignore)
                std::getline(ss, token, ',');

                // price
                std::getline(ss, token, ',');
                if (token.empty()) continue;
                t.price = std::stod(token);

                // quantity
                std::getline(ss, token, ',');
                if (token.empty()) continue;
                t.quantity = (int)std::stod(token);

                // quoteQty (ignore)
                std::getline(ss, token, ',');

                // timestamp
                std::getline(ss, token, ',');
                if (token.empty()) continue;
                t.timestamp = std::stoull(token);
                t.timestamp *= 1000000;  // ms → ns

                // isBuyerMaker
                std::getline(ss, token, ',');
                if (token.empty()) continue;

                bool isBuyerMaker = (token == "true" || token == "True");

                t.side = isBuyerMaker ? Side::ASK : Side::BID;
                t.type = EventType::ADD;

                ticks.push_back(t);
            }
            catch (...) {
                continue; // 🔥 skip bad rows safely
            }
        }
        return ticks;
    }
};