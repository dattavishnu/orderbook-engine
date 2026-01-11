# Order Book Matching Engine (C++20)

A simple, deterministic **limit order book and matching engine** implemented in **modern C++ (C++20+)**.  
Supports **Good-Till-Cancel (GTC)** and **Fill-and-Kill (FAK)** orders with FIFO price-time priority.

---

## ✨ Features

- ✅ Buy / Sell limit orders
- ✅ Price–time priority (FIFO at same price)
- ✅ Order types:
  - **Good-Till-Cancel (GTC)**
  - **Fill-and-Kill (FAK)** (partial fills allowed, remainder cancelled)
- ✅ Deterministic matching logic
- ✅ Level-2 (L2) order book snapshot
- ✅ Order cancellation
- ✅ Thread-safe public API (mutex-protected)
- ✅ Unit tested with **GoogleTest**
- ✅ Uses **C++20** features (`std::format`, `contains`, etc.)

---

## 🧠 Order Type Semantics

### Good-Till-Cancel (GTC)
- Order rests on the book until:
  - Fully filled, or
  - Explicitly cancelled

### Fill-and-Kill (FAK)
- Executes **immediately**
- Matches **as much quantity as available**
- Cancels any remaining quantity
- Never rests on the book

---

## 🏗️ Core Components

- `order` — Represents a single order
- `order_book` — Manages bids, asks, matching, and cancellations
- `trade` — Represents executed trades
- `order_level_info` — L2 snapshot (aggregated depth)

---

## 📦 Build Requirements

- **C++ compiler with C++20 support**
  - MSVC 19.29+
  - GCC 11+
  - Clang 13+
- **CMake ≥ 3.20**
- **GoogleTest**

---
