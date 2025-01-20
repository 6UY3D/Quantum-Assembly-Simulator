# Simulator Architecture

This document describes the key components and design choices behind the **Quantum Assembly Simulator**. Its aim is to provide insights into how the simulator handles state vectors, gate application, and overall workflow.

## 1. High-Level Overview
1. **Assembly Parser**: Reads `.qasm` files, tokenizes instructions, and interprets them into simulator commands.
2. **Core Engine**: Manages qubit states, applies gates, and handles measurements.
3. **Backend Optimizations**: Includes circuit optimizers, parallel execution support, and specialized memory management.
4. **User Interface**: Offers example programs, logging, and command-line usage.

## 2. Data Flow
```plaintext
.qasm File
   ↓
[ Lexer & Parser ] → [ Abstract Syntax Tree/Instruction List ]
   ↓
[ Interpreter ]    → [ Core Engine ]
   ↓
[ State Vector Updates ]
   ↓
[ Measurement & Results ]
```

## 3. State Vector Representation
- Data Structure:
  - An array (or pointer) of size 2^𝑛 for 𝑛 qubits.
  - Each element holds a complex amplitude (real and imaginary parts).
- Assembly Optimization:
  - Certain low-level operations are written in assembly to optimize matrix-vector multiplications, gate applications, etc.

