#!/usr/bin/env bash
#
# build.sh
# 
# A simple script to compile all source files into the 'quantum_simulator' executable.
# 
# Usage:
#   ./scripts/build.sh
#

# --- Configuration ---
CC="gcc"
CFLAGS="-Wall -Wextra -O3 -msse4.2 -pthread"
INCLUDES="-Isrc/core -Isrc/assembly -Isrc/backend -Isrc/utils"
OUTPUT="quantum_simulator"

echo "[Build] Starting build process..."

# 1) Clean up old artifacts
rm -f ./*.o
rm -f $OUTPUT

# 2) Compile core modules
$CC $CFLAGS $INCLUDES -c src/core/qubit.c src/core/state_vector.c src/core/gate_operations.c src/core/measurement.c

# 3) Compile assembly modules
$CC $CFLAGS $INCLUDES -c src/assembly/lexer.c src/assembly/parser.c src/assembly/interpreter.c

# 4) Compile backend modules
$CC $CFLAGS $INCLUDES -c src/backend/circuit_optimizer.c src/backend/parallel_execution.c src/backend/memory_management.c

# 5) Compile utils
$CC $CFLAGS $INCLUDES -c src/utils/file_io.c src/utils/logger.c src/utils/math_utils.c

# (Optionally) compile tests here if you want a separate test runner:
# $CC $CFLAGS $INCLUDES -c src/tests/test_core.c src/tests/test_assembly.c src/tests/test_backend.c

# 6) Link all object files into one executable
$CC $CFLAGS *.o -o $OUTPUT

# 7) Cleanup .o files if you don't want them
# rm -f ./*.o

if [ -f "$OUTPUT" ]; then
    echo "[Build] Successfully built '$OUTPUT'."
else
    echo "[Build] ERROR: Failed to create '$OUTPUT'."
    exit 1
fi
