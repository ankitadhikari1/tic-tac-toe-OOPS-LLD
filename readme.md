

````markdown
# 🎮 Tic-Tac-Toe (C++ OOP + Minimax AI)

An interactive Tic-Tac-Toe game built in C++ with clean Object-Oriented Design (LLD concepts) and an AI opponent powered by the Minimax algorithm with Alpha–Beta pruning.

---

## ✨ Features
- Human vs AI gameplay (choose X or O).
- AI opponent that never loses:
  - Uses Minimax Algorithm with Alpha–Beta pruning.
  - Prioritizes faster wins and delays losses.
- OOP / LLD Design:
  - `Board` → Manages game state.
  - `Player` (abstract) → Base class for players.
  - `HumanPlayer` → Handles user input.
  - `AIPlayer` → Implements Minimax algorithm.
  - `Game` → Orchestrates turns, rules, and result checking.
- Clean CLI interface with numbered grid.
- Extensible Design → easy to add difficulty levels, larger boards, or GUI.

---

## 🛠️ Tech Stack
- Language: C++17
- Concepts Used: OOP, Low-Level Design, Minimax Algorithm, Alpha–Beta Pruning

---

## 🚀 Getting Started

### 1. Clone the repository
```bash
git clone https://github.com/your-username/tic-tac-toe-ai.git
cd tic-tac-toe-ai
````

### 2. Build the project

```bash
g++ -std=c++17 -O2 -o ttt_oop tic_tac_toe_oop.cpp
```

### 3. Run the game

```bash
./ttt_oop
```

---

## 🕹️ How to Play

1. Choose your symbol: `X` or `O`.
   X always moves first.
2. Choose who starts the game (`X` or `O`).
3. Input a number \[1–9] to place your mark in the grid:

```
 1 | 2 | 3
---+---+---
 4 | 5 | 6
---+---+---
 7 | 8 | 9
```

4. Play until someone wins 🎉 or the game ends in a draw 🤝.

---

## 🧠 Minimax Algorithm (with Alpha–Beta Pruning)

* The AI explores all possible moves and minimizes the worst-case outcome.
* Alpha–Beta pruning reduces the number of branches explored, making it efficient.
* Evaluation:
  +10 → AI wins
  -10 → Human wins
  0 → Draw

---

## 📂 Project Structure

```
tic-tac-toe-ai/
│── tic_tac_toe_oop.cpp   # Main source code
│── README.md             # Project documentation
```

---

## 📌 Example Gameplay

```
=== Tic-Tac-Toe (OOP + Minimax with Alpha-Beta) ===
Choose your symbol (X/O). X moves first: X
Who plays first? (X/O) [default X]: O

   |   |        (1 | 2 | 3)
---+---+---
   |   |        (4 | 5 | 6)
---+---+---
   |   |        (7 | 8 | 9)

AI (O) plays at cell 5.

   |   |        (1 | 2 | 3)
---+---+---
   | O |        (4 | 5 | 6)
---+---+---
   |   |        (7 | 8 | 9)
```

---

## 🔮 Future Enhancements

* Add difficulty levels (limit search depth).
* Extend to 4x4 Tic-Tac-Toe or Connect Four.
* Build a GUI version with SFML or Qt.
* Online multiplayer mode.

---

## 📝 License

This project is licensed under the MIT License. Feel free to use, modify, and distribute.

---

👨‍💻 Developed by Ankit Adhikari

