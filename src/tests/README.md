# Tests for Quantum Assembly Simulator

This folder contains test files to ensure each component of the simulator works correctly and consistently. The tests are separated by major subsystem:

- **test_core.c**: Covers `qubit.c`, `state_vector.c`, `gate_operations.c`, and `measurement.c`.
- **test_assembly.c**: Covers the lexer, parser, and interpreter in the `src/assembly/` folder.
- **test_backend.c**: Covers the circuit optimizer, parallel execution, and memory management in the `src/backend/` folder.

## Building the Tests

Below is a sample command-line approach using GCC on Linux. Adjust paths and flags for your environment (Windows, macOS, or other build systems like CMake):

```bash
cd src/tests

# Example of compiling core tests:
gcc -O3 -msse4.2 -I../core -I../assembly -I../backend \
    -pthread \
    -c ../core/qubit.c ../core/state_vector.c ../core/gate_operations.c ../core/measurement.c \
       ../assembly/lexer.c ../assembly/parser.c ../assembly/interpreter.c \
       ../backend/circuit_optimizer.c ../backend/parallel_execution.c ../backend/memory_management.c

gcc -o test_core test_core.c *.o -lpthread

# Run the test_core executable
./test_core

# Similarly for test_assembly.c and test_backend.c
gcc -o test_assembly test_assembly.c *.o -lpthread
./test_assembly

gcc -o test_backend test_backend.c *.o -lpthread
./test_backend
