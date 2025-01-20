# User Guide

This guide walks you through using the Quantum Assembly Simulator, from building the project to running quantum assembly programs.

## Table of Contents
1. [Installation](#installation)
2. [Building](#building)
3. [Running the Simulator](#running-the-simulator)
4. [Writing Quantum Assembly Code](#writing-quantum-assembly-code)
5. [Interpreting Results](#interpreting-results)
6. [Advanced Usage](#advanced-usage)

---

## Installation

1. **Clone** the repository:
   ```bash
   git clone https://github.com/6UY3D/quantum-assembly-simulator.git
2. **Navigate** into the project directory:
   ```bash
   cd quantum-assembly-simulator
3. (Optional) **Install** any dependencies or libraries required for your assembly optimizations. Refer to your system’s package manager for details.

## Building
1. **Compile** the project using the provided script:
   ```bash
   ./scripts/build.sh
    ```
   This will create an executable (e.g., quantum_simulator) in the project root or a bin/ directory, depending on how your build script is set up.
2. **Verify** the build by running tests:
   ```bash
   cd src/tests
   # Example command to compile and run tests (depends on your test structure)
   make test
   ./test_runner

## Running the Simulator
Use the run.sh script in the scripts/ folder to execute quantum assembly programs. For example:
   ```bash
   ./scripts/run.sh examples/hello_world.qasm
   ```

This script:

1. Parses the .qasm file
2. Interprets the instructions
3. Simulates the quantum circuit
4. Outputs the final state or measurement result

## Writing Quantum Assembly Code
Our simulator expects quantum assembly files (e.g., .qasm) with a simple syntax:

- Gate Commands: E.g., H 0 applies a Hadamard gate to qubit 0, CNOT 0 1 applies a controlled NOT with control qubit 0 and target qubit 1.
- Measurements: E.g., MEASURE 0 instructs the simulator to measure qubit 0.
- Comments: Start with // (or #, depending on your preference).

**Example:**
   ```plaintext
   // Initialize a single qubit in the |0> state
   H 0
   X 0
   MEASURE 0
   ```
For more examples, see the examples/ folder.

## Interpreting Results
1. **State-Vector Output:** The simulator may print out the final amplitudes of each basis state.
2. **Measurement Results:** If your code contains measurement instructions, the simulator will output the probabilities or a “collapsed” result of 0/1 for each measured qubit.

## Advanced Usage
- Circuit Optimization: The simulator automatically optimizes circuits if you enable the feature (see circuit_optimizer.c).
- Parallel Execution: For large numbers of qubits, enable multithreading in parallel_execution.c (subject to hardware limits).
- Memory Management: Tweak buffer sizes and memory strategies in memory_management.c to handle bigger circuits.

