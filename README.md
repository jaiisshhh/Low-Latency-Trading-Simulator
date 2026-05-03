# ⚡ Low-Latency Trading Simulator (C++)

A high-performance, event-driven trading system built in C++ that simulates real-time market data processing with microsecond-level latency.
Designed to demonstrate core **quant developer skills**: low-latency systems, lock-free programming, efficient data structures, and performance benchmarking.

---

## 🚀 Overview

This project simulates a simplified trading infrastructure:

```
Market Data → Lock-Free Queue → Processor → Order Book → Strategy
```

It processes millions of ticks per second while maintaining extremely low latency, making it a strong representation of systems used in **high-frequency trading (HFT)** environments.

Additionally, a **Python-based backtesting module** is included to validate strategies on real market data.

---

## 🧠 Key Features

### ⚙️ System Design

- Event-driven architecture
- Multi-threaded producer-consumer pipeline
- Lock-free Single Producer Single Consumer (SPSC) queue

### ⚡ Performance

- ~10M ticks/sec throughput
- ~40ns P50 latency
- Real-time latency tracking (P50 / P99 / P999)

### 📊 Order Book

- O(1) price-indexed order book
- Best bid/ask tracking
- Matching engine simulation

### 📈 Strategy Layer

- Flow imbalance-based trading strategy
- Real-time execution (buy/sell simulation)
- PnL tracking

### 📉 Backtesting (Python)

- Replay real Binance tick data
- Strategy validation on historical data
- Equity curve + drawdown analysis

### 📡 Real Market Data

- Integrated Binance BTCUSDT trade data
- Historical tick replay

---

## 🏗️ Architecture

### 🔹 Components

#### 1. Tick Generator / Data Loader

- Generates synthetic ticks OR loads real Binance data
- Each tick contains:
  - Timestamp
  - Price
  - Quantity
  - Side (BID/ASK)

---

#### 2. Lock-Free Queue (`SPSCQueue`)

- Ring buffer implementation
- Wait-free push/pop
- Avoids mutex overhead
- Cache-friendly design

---

#### 3. Processor

- Core engine loop
- Consumes ticks from queue
- Updates order book
- Executes strategy
- Tracks latency + throughput

---

#### 4. Order Book

- Array-based (price-indexed)
- O(1) updates
- Maintains:
  - Best bid
  - Best ask

- Matching engine logic for trades

---

#### 5. Strategy

- Trade-flow imbalance signal
- Entry/exit logic
- Position management
- PnL calculation

---

## 📊 Performance Metrics

| Metric       | Value          |
| ------------ | -------------- |
| Throughput   | ~10M ticks/sec |
| P50 Latency  | ~40 ns         |
| P99 Latency  | ~40–200 ns     |
| P999 Latency | ~80–300 ns     |

### Benchmark Results

```
=== Running with 10M ticks ===
Throughput: ~10,000,000 ticks/sec

Latency Stats:
P50  : ~41 ns
P99  : ~42 ns
P999 : ~84 ns
```

### 📈 Sample Backtest Result

```
Trades       : 103
Position     : 3
Cash         : -231474.55
PnL          : 3223.10
Max Drawdown : 3473.76
```

### 📊 Equity Curve

- Shows cumulative PnL over time
- Highlights:
  - Profitability trends
  - Drawdowns
  - Strategy stability

---

### ⚠️ Notes

- No transaction costs or slippage
- Strategy is **for validation, not production trading**
- Focus is on **system + pipeline correctness**

---

## ⚡ Performance Optimizations

- Pre-allocated memory (no dynamic allocations in hot path)
- Cache-friendly contiguous arrays
- Lock-free communication (SPSC queue)
- Minimal branching in critical loop
- Inline functions for hot paths
- Avoided STL containers in latency-sensitive code

---

## 🧵 Concurrency Model

- **Producer Thread**
  - Generates / loads ticks
  - Pushes into queue

- **Consumer Thread**
  - Processes ticks
  - Updates system state

👉 Ensures high throughput with minimal contention.

---

## 📉 Strategy Logic (Simplified)

- Uses **trade flow imbalance**:

  ```
  imbalance = (buy_volume - sell_volume) / total_volume
  ```

- Entry:
  - Buy when imbalance is strongly positive
  - Sell when imbalance is strongly negative

- Exit:
  - Based on price reversal / movement thresholds

⚠️ Note: Strategy is for demonstration purposes and not optimized for profitability.

---

## 📡 Data Source

- Binance BTCUSDT trade data
- Parsed into internal tick format

---

## 🛠️ Build & Run

### Compile

```bash
clang++ -std=c++17 main.cpp tick_generator.cpp -pthread -O3 -o main
```

### Run

```bash
./main
```

### Run Backtest (Python)
```bash
cd Backtesting
python3 backtest.py
```
---

## 📂 Project Structure

```
├── main.cpp
├── processor.h
├── order_book.h
├── strategy.h
├── latency_tracker.h
├── lockfree_queue.h
├── tick_generator.cpp
├── types.h
├── data_loader.h
│
├── Backtesting/
│   └── backtest.py
│
├── market_data/
```

---

## 🧠 Key Learnings

- Designing low-latency systems requires:
  - careful memory management
  - minimizing locks and allocations
  - cache-aware data structures

- Trade data ≠ order book data

- Microstructure signals are noisy and require filtering

- Strategy validation requires separate offline backtesting

---

## 🚀 Future Improvements

- Real Level 2 (order book depth) integration
- Queue position modeling
- Slippage & execution simulation
- CPU affinity / thread pinning
- False sharing optimization
- Advanced strategy (momentum + volatility + spread filters)

---

## 🎯 Why This Project Matters

This project demonstrates:

- Systems-level thinking
- Low-latency performance engineering
- Concurrency and lock-free design
- Real-world data handling
- End-to-end trading system design
- Strategy validation pipeline (C++ + Python)

---

## ⭐ Summary

A high-performance, low-latency trading simulator that processes millions of market events per second — showcasing practical skills required in modern trading infrastructure.

---
