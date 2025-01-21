#!/usr/bin/env bash
#
# run.sh
#
# Runs the quantum simulator on a specified .qasm file.
#
# Usage:
#   ./scripts/run.sh path/to/program.qasm
#

SIMULATOR="./quantum_simulator"  # The executable from build.sh

if [ ! -f "$SIMULATOR" ]; then
    echo "[Run] ERROR: '$SIMULATOR' not found. Please run build.sh first."
    exit 1
fi

if [ $# -lt 1 ]; then
    echo "Usage: $0 path/to/file.qasm"
    exit 2
fi

QASM_FILE="$1"

# Check if the file exists
if [ ! -f "$QASM_FILE" ]; then
    echo "[Run] ERROR: QASM file '$QASM_FILE' not found."
    exit 3
fi

# Execute the simulator
echo "[Run] Executing '$QASM_FILE'..."
"$SIMULATOR" "$QASM_FILE"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    echo "[Run] Simulator finished successfully."
else
    echo "[Run] Simulator exited with code $EXIT_CODE."
fi

exit $EXIT_CODE
