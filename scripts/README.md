# Scripts for Quantum Assembly Simulator

This folder contains shell scripts that facilitate building and running the Quantum Assembly Simulator.

1. **build.sh**  
   - Compiles all source files across `src/core/`, `src/assembly/`, `src/backend/`, and `src/utils/` into a single executable (e.g., `quantum_simulator`).

2. **run.sh**  
   - Executes the compiled simulator with a specified `.qasm` file.
   - Automatically checks if the `.qasm` file exists before running.

## Usage

```bash
# 1) Build the project
./scripts/build.sh

# 2) Run a .qasm file, e.g.:
./scripts/run.sh examples/hello_world.qasm
