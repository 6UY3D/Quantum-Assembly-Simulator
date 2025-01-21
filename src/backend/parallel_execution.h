c

#ifndef PARALLEL_EXECUTION_H
#define PARALLEL_EXECUTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/state_vector.h"

/**
 * \brief Parallelize the application of a single-qubit gate to a large state vector using threads.
 * \param sv The state vector to modify
 * \param gate A 2x2 gate matrix in float format
 * \param qubit_index The qubit on which the gate is applied
 * \param num_threads Number of threads to spawn
 * \return 0 on success, nonzero on error
 */
int parallel_apply_single_qubit_gate(StateVector* sv, const float* gate, size_t qubit_index, int num_threads);

/**
 * \brief Parallel measure all qubits in a state vector (for demonstration).
 * \param sv The state vector
 * \param results Pointer to an array of int of length sv->num_qubits
 * \param num_threads Number of threads
 * \return 0 on success, nonzero on error
 *
 * **Note**: This is not exactly "physical" since measuring all qubits simultaneously
 * typically requires a single pass. This example shows how to distribute the partial
 * measurement & collapse among threads. Real logic might differ depending on your approach.
 */
int parallel_measure_all(StateVector* sv, int* results, int num_threads);

#ifdef __cplusplus
}
#endif

#endif /* PARALLEL_EXECUTION_H */
