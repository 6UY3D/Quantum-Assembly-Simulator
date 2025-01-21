```plaintext
// hello_world.qasm
// A minimal example creating a superposition on a single qubit and measuring it.

// Start with qubit 0 in state |0>
H 0           // Apply Hadamard gate, creating (|0> + |1>) / sqrt(2)
MEASURE 0     // Measure the qubit
