# 🌌 Enchanted Labyrinth Explorer

Welcome to the mystical realm of **Arcadia**, where the ancient **Enchanted Labyrinth** awaits! Venture into a procedurally generated maze filled with traps, magical creatures, and hidden treasures. Use your wits, weapons, and magical items to survive and claim the riches that lie within.

---

## 🕹️ Gameplay Overview

- 🧙‍♂️ **You are an adventurer** exploring a dangerous, dynamic labyrinth.
- 🗺️ **Procedurally generated mazes** ensure a unique experience each time.
- 🧟 **Enemies roam the labyrinth** — defeat them using collected weapons and strategy.
- 💎 **Treasure collection system** managed by a custom AVL Tree data structure.
- 🧠 **Pathfinding assistance** via Dijkstra’s algorithm to guide your journey.
- ⚔️ **Combat system** allows real-time battles with enemy creatures.
- 🏆 **Score and reward system** encourages deeper exploration for rarer loot.

---

## 🎯 Features

- ✅ **AVL Tree Inventory**: Efficient insert, delete, and search for collected items.
- ✅ **Random Maze Generation**: DFS-based algorithm ensures unique layouts.
- ✅ **Shortest Path Algorithm**: Smart navigation using Dijkstra’s algorithm.
- ✅ **Enemy AI & Bullet Mechanics**: Real-time danger from magical creatures.
- ✅ **Combat Mechanics**: Fight with swords, shields, and magic potions.
- ✅ **Treasure Tracking**: Rewards with item effects (shield, key, buffs).
- ✅ **Immersive UI**: Score display, inventory showcase, and game-over screens.
- ✅ **Pause Feature**: Pause anytime with the `P` key.


---

## 🛠️ How to Build

### 🔗 Dependencies

- **C++17** (or higher)
- **SFML 2.6.1**

### 🧩 SFML Setup
1. Download SFML: [SFML 2.6.1](https://www.sfml-dev.org/download/sfml/2.6.1/)
2. Follow the setup tutorial: [SFML + Visual Studio Setup](https://youtu.be/pgvlJ-Zr9Ys?si=gH4AebmA5nQEfJdy)

### 🧪 Compilation Instructions

Using **g++** (adjust SFML path as needed):
```bash
g++ -std=c++17 main.cpp -o LabyrinthExplorer -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
