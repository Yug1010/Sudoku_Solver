# Sudoku Solver

A Sudoku solver built in **C++** with a **Node.js + Express** backend and a dark-themed interactive frontend. Uses the **Backtracking + MRV (Minimum Remaining Values)** algorithm with Forward Checking.

---

## Project Structure

```
sudoku-project/
├── sudoku_mrv.cpp       # C++ solver (compile this)
├── server.js            # Node.js API server
├── package.json         # Node dependencies
├── .gitignore
└── public/
    └── index.html       # Frontend UI
```

---

## Algorithm

### Backtracking + MRV (Minimum Remaining Values)

Plain backtracking scans cells left-to-right and tries digits 1–9 at each empty cell, undoing choices that lead to dead ends. MRV improves this by always picking the empty cell with the **fewest valid candidates** first.

**Why MRV works:**
- A cell with 1 candidate is filled instantly with no branching
- Detecting constrained cells early collapses large portions of the search tree
- Built-in **Forward Checking** — if any empty cell reaches 0 candidates during the scan, the solver backtracks immediately without recursing further

**Why not Dynamic Programming:**
Sudoku is a **Constraint Satisfaction Problem (CSP)**, not an optimization problem. DP requires overlapping subproblems that can be cached — every Sudoku board state is unique, so there is nothing to cache. Backtracking is the theoretically correct approach.

**Performance comparison on a hard puzzle:**

| Approach | Recursive Calls |
|---|---|
| Plain Backtracking | ~5,000,000 |
| Backtracking + MRV | ~200 |

### Validation

Before solving, `isValidInput()` checks all given clues against each other. For each filled cell it temporarily removes the value and checks whether placing it back violates any row, column, or box constraint. Invalid puzzles are rejected before the solver runs.

---

## Getting Started

### Prerequisites

- **g++** (C++ compiler)
- **Node.js** v14 or higher
- **npm**

### 1. Clone the repository

```bash
git clone https://github.com/your-username/sudoku-solver.git
cd sudoku-solver
```

### 2. Compile the C++ solver

```bash
g++ -o sudoku sudoku_mrv.cpp
```

This creates a `sudoku` binary that the server spawns at runtime.

### 3. Install Node dependencies

```bash
npm install
```

### 4. Start the server

```bash
node server.js
```

### 5. Open the app

Visit `http://localhost:3000` in your browser.

---

## How the Frontend and Backend Connect

```
Browser (index.html)
      │
      │  POST /solve
      │  Body: { grid: [81 numbers] }
      ▼
Node.js server (server.js)
      │
      │  spawn('./sudoku')
      │  stdin ← 81 numbers
      ▼
C++ binary (sudoku_mrv.cpp)
      │
      │  stdout → solved grid display
      ▼
Node.js parses output
      │
      │  Response: { solved: true, grid: [81 numbers] }
      ▼
Browser renders solution
```

The C++ binary reads 81 space-separated integers from `stdin` (0 for empty cells) and writes the solved grid to `stdout` using the same format as `display()`. The Node server parses the digit output and returns it as JSON.

---

## Running the C++ Solver Directly

You can also run the solver standalone without the server:

```bash
./sudoku
```

Enter the puzzle row by row, 9 digits per row, with 0 for empty cells:

```
5 3 0 0 7 0 0 0 0
6 0 0 1 9 5 0 0 0
0 9 8 0 0 0 0 6 0
8 0 0 0 6 0 0 0 3
4 0 0 8 0 3 0 0 1
7 0 0 0 2 0 0 0 6
0 6 0 0 0 0 2 8 0
0 0 0 4 1 9 0 0 5
0 0 0 0 8 0 0 7 9
```

---

## Frontend Features

- Interactive 9×9 grid — click cells and type, or use the numpad
- Arrow key navigation across the grid
- **Live conflict detection** — conflicting cells turn red as you type
- **MRV animation** — watch the algorithm pick cells and backtrack in real time
- 5 speed modes: Animated → Slow → Normal → Fast → Instant
- Stats display: recursive calls, backtracks, MRV picks
- Color legend: given clues, MRV pick, placed digits, backtracks

---

## Input Format

| Value | Meaning |
|---|---|
| 1–9 | Pre-filled clue |
| 0 | Empty cell |

The grid must be exactly 81 values. Any puzzle where two identical digits appear in the same row, column, or 3×3 box is rejected as invalid before solving.

---

## Technologies Used

| Layer | Technology |
|---|---|
| Algorithm | C++ (no STL vectors) |
| Backend | Node.js, Express |
| Frontend | Vanilla HTML/CSS/JS |
| Fonts | Playfair Display, DM Mono |

---

## License

MIT