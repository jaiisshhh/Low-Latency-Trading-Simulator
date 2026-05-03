import pandas as pd
import matplotlib.pyplot as plt

# ===== LOAD DATA =====
df = pd.read_csv(
    "../market_data/BTCUSDT-trades-2026-05-01.csv",
    header=None,
    names=[
        "trade_id",
        "price",
        "qty",
        "quote_qty",
        "timestamp",
        "isBuyerMaker",
        "isBestMatch"
    ]
)

# ===== STATE =====
position = 0
cash = 0
last_price = 0

buy_volume = 0
sell_volume = 0

trades = 0
equity = []

# 🔥 NEW: controls
MAX_POSITION = 3
COOLDOWN = 1000          # ticks between trades
MIN_PRICE_MOVE = 5       # ignore tiny moves

last_trade_index = -COOLDOWN

# ===== MAIN LOOP =====
for i, row in df.iterrows():
    try:
        price = float(row["price"])
        qty = float(row["qty"])
        is_maker = row["isBuyerMaker"]
    except:
        continue

    # ===== PRICE FILTER =====
    if abs(price - last_price) < MIN_PRICE_MOVE:
        continue

    # ===== FLOW CLASSIFICATION =====
    if is_maker:
        sell_volume += qty
    else:
        buy_volume += qty

    total = buy_volume + sell_volume
    if total < 0.01:
        continue

    imbalance = (buy_volume - sell_volume) / total

    # ===== COOLDOWN CHECK =====
    if i - last_trade_index < COOLDOWN:
        last_price = price
        continue

    # ===== STRATEGY =====
    if imbalance > 0.1 and position < MAX_POSITION:
        position += 1
        cash -= price
        trades += 1
        last_trade_index = i
        buy_volume = sell_volume = 0

    elif imbalance < -0.1 and position > -MAX_POSITION:
        position -= 1
        cash += price
        trades += 1
        last_trade_index = i
        buy_volume = sell_volume = 0

    last_price = price

    # ===== EQUITY TRACKING =====
    running_pnl = cash + position * price
    equity.append(running_pnl)

# ===== FINAL PNL =====
pnl = cash + position * last_price

# ===== DRAWDOWN =====
peak = float('-inf')
max_drawdown = 0

for value in equity:
    if value > peak:
        peak = value
    drawdown = peak - value
    if drawdown > max_drawdown:
        max_drawdown = drawdown

# ===== REPORT =====
print("\n===== BACKTEST REPORT =====")
print(f"Trades        : {trades}")
print(f"Position      : {position}")
print(f"Cash          : {cash:.2f}")
print(f"PnL           : {pnl:.2f}")
print(f"Max Drawdown  : {max_drawdown:.2f}")

# ===== EQUITY CURVE =====
plt.figure(figsize=(10, 5))
plt.plot(equity)
plt.title("Equity Curve")
plt.xlabel("Ticks")
plt.ylabel("PnL")
plt.grid(True)
plt.show()