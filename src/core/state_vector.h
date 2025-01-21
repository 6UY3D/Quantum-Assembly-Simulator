#ifndef STATE_VECTOR_H
#define STATE_VECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/**
 * \brief Structure for multi-qubit state vector.
 *        The vector has length 2^num_qubits for real part and 2^num_qubits for imaginary part.
 */
typedef struct StateVector {
    size_t num_qubits;  /**< Number of qubits in this system */
    float* real;        /**< Real parts of the amplitudes */
    float* imag;        /**< Imag parts of the amplitudes */
} StateVector;

/**
 * \brief Allocates and initializes a StateVector in the |0...0> state.
 * \param sv Pointer to a StateVector struct
 * \param num_qubits Number of qubits
 * \return 0 on success, nonzero on error
 */
int init_state_vector(StateVector* sv, size_t num_qubits);

/**
 * \brief Frees resources associated with a StateVector.
 * \param sv Pointer to a StateVector struct
 */
void free_state_vector(StateVector* sv);

/**
 * \brief Prints a subset of the amplitudes (for debugging).
 * \param sv Pointer to the StateVector
 * \param max_entries Maximum number of entries to print (e.g., 16).
 */
void print_state_vector(const StateVector* sv, size_t max_entries);

#ifdef __cplusplus
}
#endif

#endif /* STATE_VECTOR_H */
