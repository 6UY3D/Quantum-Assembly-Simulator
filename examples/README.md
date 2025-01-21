# Quantum Assembly Simulator Examples

This folder contains example `.qasm` programs demonstrating how to use the Quantum Assembly Simulator's custom assembly language. Each example showcases a different concept in quantum computing, from basic single-qubit operations to more sophisticated algorithms.

## File Overview

1. **hello_world.qasm**  
   Creates a simple superposition on one qubit, then measures the result—akin to a “Hello World” for quantum assembly.

2. **quantum_teleportation.qasm**  
   Demonstrates the essential steps of quantum teleportation on three qubits, showing how to prepare an entangled pair, perform bell-basis measurements, and (optionally) correct the final state.

3. **grovers_algorithm.qasm**  
   Implements a simplified version of Grover’s search for a two-qubit “oracle,” illustrating how to apply the oracle and diffusion operators in a small system.

## Usage

After building the simulator:

```bash
./scripts/run.sh examples/hello_world.qasm
./scripts/run.sh examples/quantum_teleportation.qasm
./scripts/run.sh examples/grovers_algorithm.qasm
