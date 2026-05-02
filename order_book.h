#pragma once
#include "types.h"
#include <cmath>

class alignas(64) OrderBook {
public:
    static const int MAX_PRICE = 20000; // up to 200.00
    static const int TICK_SCALE = 100;

    OrderBook() {
        for (int i = 0; i < MAX_PRICE; i++) {
            bid_qty[i] = 0;
            ask_qty[i] = 0;
        }
        best_bid = -1;
        best_ask = MAX_PRICE;
    }

    // 🔥 CORRECT ACCESSORS
    double get_best_bid() const {
        return (best_bid >= 0) ? best_bid / (double)TICK_SCALE : 0.0;
    }

    double get_best_ask() const {
        return (best_ask < MAX_PRICE) ? best_ask / (double)TICK_SCALE : 0.0;
    }

    int get_best_bid_qty() const {
        return (best_bid >= 0) ? bid_qty[best_bid] : 0;
    }

    int get_best_ask_qty() const {
        return (best_ask < MAX_PRICE) ? ask_qty[best_ask] : 0;
    }

    void update(const Tick& tick) {
        int price_idx = (int)(tick.price * TICK_SCALE);

        if (price_idx < 0 || price_idx >= MAX_PRICE) return;

        if (tick.side == Side::BID) {
            bid_qty[price_idx] += tick.quantity;

            if (price_idx > best_bid) {
                best_bid = price_idx;
            }
        } else {
            ask_qty[price_idx] += tick.quantity;

            if (price_idx < best_ask) {
                best_ask = price_idx;
            }
        }

        // 🔥 match after update
        match();
    }

private:
    int bid_qty[MAX_PRICE];
    int ask_qty[MAX_PRICE];

    int best_bid;
    int best_ask;

    void match() {
        while (best_bid >= 0 &&
               best_ask < MAX_PRICE &&
               best_bid >= best_ask) {

            int qty = std::min(bid_qty[best_bid], ask_qty[best_ask]);

            bid_qty[best_bid] -= qty;
            ask_qty[best_ask] -= qty;

            if (bid_qty[best_bid] == 0) {
                int scan = 0;
                while (best_bid >= 0 && bid_qty[best_bid] == 0 && scan < 10) {
                    best_bid--;
                    scan++;
                }
            }

            if (ask_qty[best_ask] == 0) {
                int scan = 0;
                while (best_ask < MAX_PRICE && ask_qty[best_ask] == 0 && scan < 10) {
                    best_ask++;
                    scan++;
                }
            }
        }
    }
};