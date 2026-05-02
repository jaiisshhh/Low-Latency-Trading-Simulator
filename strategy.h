#pragma once
#include "order_book.h"
#include <cstdio>

class Strategy
{
public:
    double buy_volume = 0;
    double sell_volume = 0;
    double last_price = 0;

    void on_tick(const Tick &tick, const OrderBook &book)
    {
        double price = tick.price;

        if (last_price != 0)
        {
            double move = std::abs(price - last_price);

            // BTC scale filter (~$10 move)
            if (move < 10.0)
                return;
        }

        if (tick.side == Side::BID)
            buy_volume += tick.quantity;
        else
            sell_volume += tick.quantity;

        double total = buy_volume + sell_volume;
        if (total < 100)
            return;

        double imbalance = (buy_volume - sell_volume) / total;

        // 🔥 ENTRY (NO momentum filter here)
        if (imbalance > 0.2 && position < max_position)
        {
            execute_buy(price);
        }
        else if (imbalance < -0.2 && position > -max_position)
        {
            execute_sell(price);
        }

        // only exit on meaningful reversal
        if (position > 0 && price < last_price - 10.0)
        {
            execute_sell(price);
        }
        else if (position < 0 && price > last_price + 10.0)
        {
            execute_buy(price);
        }

        // decay
        buy_volume *= 0.9;
        sell_volume *= 0.9;

        last_price = price;
    }

    void report()
    {
        double pnl = cash + position * last_price;

        printf("\n = STRATEGY REPORT =\n");
        printf("Position: %.2f\n", position);
        printf("Cash    : %.2f\n", cash);
        printf("PnL     : %.2f\n", pnl);
        printf("\n");
    }

    void reset_flow()
    {
        buy_volume = 0;
        sell_volume = 0;
    }

private:
    double position = 0.0;
    double cash = 0.0;
    double prev_mid = 0.0;

    int prev_bid_qty = 0;
    int prev_ask_qty = 0;

    const double threshold = 0.01;
    const int max_position = 10;
    const int liquidity_threshold = 20; // 🔥 KEY

    void execute_buy(double price)
    {
        position += 1;
        cash -= price;
        printf("BUY @ %.2f\n", price);
    }

    void execute_sell(double price)
    {
        position -= 1;
        cash += price;
        printf("SELL @ %.2f\n", price);
    }
};