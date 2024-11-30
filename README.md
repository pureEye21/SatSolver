# SAT Solver & Crossword Solver

## Overview

This repository contains two projects that leverage the power of SAT solving techniques in Python:

1. **SAT Solver Implementation**: A custom SAT solver implemented using the DPLL (Davis-Putnam-Logemann-Loveland) algorithm, featuring unit propagation, literal elimination, and branching heuristics.
2. **Crossword Solver**: A solver built using the PySAT library, which formulates and solves crossword puzzles by converting them into SAT clauses in Conjunctive Normal Form (CNF).

## SAT Solver

### Features
- **DPLL Algorithm**: A classic SAT-solving algorithm that recursively attempts to find a satisfying assignment for a given Boolean formula.
- **Unit Propagation**: Simplifies the formula by iteratively assigning values to literals that appear as singletons in clauses.
- **Literal Elimination**: Eliminates literals from the formula that can be easily satisfied, reducing the problem size.
- **Branching Heuristics**: Implements decision-making strategies to choose the next literal to assign, improving the efficiency of the solver.

### Usage
Clone the repository and navigate to the `sat_solver` directory to find the implementation.

```bash
g++ ./main.cpp ./solver.cpp
./a.exe
